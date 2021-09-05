/*
 * PCControl.cpp
 *
 *  Created on: Jun 15, 2021
 *      Author: rik
 */

#include "PCControl.h"
#include <string.h>

extern uint8_t globalUartReceiveBuffer[1];
extern UART_HandleTypeDef huart1;

PCControl::PCControl()
{
	m_uartBuffer.fill(0);
}

void PCControl::Init(IPCControl* PCControlCallbacks)
{
	m_callbacks = PCControlCallbacks;		//proper way is to check for nullptr but well.
}

void PCControl::Run()
{
	if(m_issuedCommand.command != Command_e::INVALID && m_callbacks)
	{
		switch (m_issuedCommand.command)
		{
		case Command_e::SET_MA:
			if(m_callbacks->SetMilliAmps(m_issuedCommand.value))
			{
				ResponseSendOK();
			}
			else
			{
				ResponseSendNOK();
			}
			break;
		case Command_e::SET_MV:
			if(m_callbacks->SetMilliVolt(m_issuedCommand.value))
			{
				ResponseSendOK();
			}
			else
			{
				ResponseSendNOK();
			}
			break;
		case Command_e::GET_MV:
		{
			auto mv = std::to_string(m_callbacks->GetMilliVolt());
			mv.append("\n", 1);
			UART_TRANSMIT(&huart1, mv.c_str(), mv.size(), 100);
		}
		break;
		case Command_e::GET_MA:
			{
				auto ma = std::to_string(m_callbacks->GetMilliAmps());
				ma.append("\n", 1);
				UART_TRANSMIT(&huart1, ma.c_str(), ma.size(), 100);
			}
			break;
		case Command_e::GET_TEMP:
		{
			auto temp = std::to_string(m_callbacks->GetTemperatureInCelcius());
			temp.append("\n", 1);
			UART_TRANSMIT(&huart1, temp.c_str(), temp.size(), 100);
		}
		break;
		case Command_e::GET_CTRL:
			m_callbacks->LockControlToPC();
			ResponseSendOK();
			break;
		case Command_e::DROP_CTRL:
			m_callbacks->UnlockControlToPC();
			ResponseSendOK();
			break;
		default:
			break;
		}
		m_issuedCommand.command = Command_e::INVALID;
	}
}

void PCControl::ProcessUartInterrupt(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		m_uartBuffer[m_uartCounter] = globalUartReceiveBuffer[0];

		if(m_uartBuffer[m_uartCounter] == '\n')
		{
			m_issuedCommand = ParseUartCommand((char*)m_uartBuffer.data(), m_uartCounter);
			ClearUartReceiveBuffer();
		}
		else
		{
			m_uartCounter++;
			if(m_uartCounter >= 64)
			{
				ClearUartReceiveBuffer();
			}
		}

		HAL_UART_Receive_IT(huart, globalUartReceiveBuffer, 1);
	}
}

void PCControl::ClearUartReceiveBuffer()
{
	m_uartBuffer.fill(0);
	m_uartCounter = 0;
}

PCControl::IssuedCommand_t PCControl::ParseUartCommand(char* data, uint8_t size)
{
	PCControl::IssuedCommand_t command = {Command_e::INVALID, 0};
	if(size >= 5)
	{
		if(!strncmp(data, "GetmV", 5))
		{
			command.command = Command_e::GET_MV;
		}
		else if(!strncmp(data, "GetmA", 5))
		{
			command.command = Command_e::GET_MA;

		}
		else if(!strncmp(data, "SetmV", 5))
		{
			command.command = Command_e::SET_MV;
			command.value = GetValueFromUartCommand(data+6, size-6);
		}
		else if(!strncmp(data, "SetmA", 5))
		{
			command.command = Command_e::SET_MA;
			command.value = GetValueFromUartCommand(data+6, size-6);
		}
	}
	if(size >= 7)
	{
		if(!strncmp(data, "GetTemp", 7))
		{
			command.command = Command_e::GET_TEMP;
		}
		else if(!strncmp(data, "GetCtrl", 7))
		{
			command.command = Command_e::GET_CTRL;
		}
	}
	if(size >= 8)
	{
		if(!strncmp(data, "DropCtrl", 8))
		{
			command.command = Command_e::DROP_CTRL;
		}
	}
	return command;
}

uint32_t PCControl::GetValueFromUartCommand(char* data, uint8_t size)
{
	return std::stoi(data, nullptr, 10);
}

void PCControl::ResponseSendOK()
{
	UART_TRANSMIT(&huart1, "OK\n", 3, 100);
}

void PCControl::ResponseSendNOK()
{
	UART_TRANSMIT(&huart1, ":(\n", 3, 100);
}

HAL_StatusTypeDef PCControl::UART_TRANSMIT(UART_HandleTypeDef *huart, const char *pData, uint16_t Size, uint32_t Timeout)
{
	return HAL_UART_Transmit(huart, reinterpret_cast<uint8_t*>(const_cast<char*>(pData)), Size, Timeout);
}
