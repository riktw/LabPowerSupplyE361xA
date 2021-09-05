/*
 * HPDL1414.cpp
 *
 *  Created on: May 12, 2021
 *      Author: rik
 */

#include "HPDL1414.h"

#include <string.h>
#include <stdio.h>

HPDL1414::HPDL1414()
{

}

void HPDL1414::Init(gpio addr0, gpio addr1, gpio wr)
{
	memcpy(&m_addr0, &addr0, sizeof(addr0));
	memcpy(&m_addr1, &addr1, sizeof(addr1));
	memcpy(&m_wr, &wr, sizeof(wr));
}

void HPDL1414::SetDigit(uint8_t digit, char data)
{
	WriteChar(data);

	switch (digit) {
	case 0:
		HAL_GPIO_WritePin(m_addr0.port, m_addr0.pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(m_addr1.port, m_addr1.pin, GPIO_PIN_SET);
		break;
	case 1:
		HAL_GPIO_WritePin(m_addr0.port, m_addr0.pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(m_addr1.port, m_addr1.pin, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(m_addr0.port, m_addr0.pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(m_addr1.port, m_addr1.pin, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(m_addr0.port, m_addr0.pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(m_addr1.port, m_addr1.pin, GPIO_PIN_RESET);
		break;
	default:
		break;
	}

	HAL_Delay(1);

	HAL_GPIO_WritePin(m_wr.port, m_wr.pin, GPIO_PIN_RESET);

	HAL_Delay(1);

	HAL_GPIO_WritePin(m_wr.port, m_wr.pin, GPIO_PIN_SET);
}

void HPDL1414::SetDisplay(char data[NUMBER_OF_DIGITS])
{
	for(int i = 0; i < NUMBER_OF_DIGITS; ++i)
	{
		SetDigit(i, data[i]);
	}
}

void HPDL1414::WriteChar(char data)
{
	if(data & (1 << 0))
		HAL_GPIO_WritePin(D0_GPIO_Port, D0_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(D0_GPIO_Port, D0_Pin, GPIO_PIN_RESET);

	if(data & (1 << 1))
		HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, GPIO_PIN_RESET);

	if(data & (1 << 2))
		HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, GPIO_PIN_RESET);

	if(data & (1 << 3))
		HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, GPIO_PIN_RESET);

	if(data & (1 << 4))
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

	if(data & (1 << 5))
		HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_RESET);

	if(data & (1 << 6))
		HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_RESET);

}

Display::Display()
{

}

void Display::Init(gpio addr0, gpio addr1, gpio wr1, gpio addr2, gpio addr3, gpio wr2)
{
	m_displays[0].Init(addr0, addr1, wr1);
	m_displays[1].Init(addr2, addr3, wr2);
}

void Display::Set(const char data[NUMBER_OF_DIGITS])
{
	// second display has middle 2 digits switched around, lemme fix it here.
	char newdata[NUMBER_OF_DIGITS];
	memcpy(newdata, data, NUMBER_OF_DIGITS);
	char temp = newdata[5];
	newdata[5] = newdata[6];
	newdata[6] = temp;
	m_displays[0].SetDisplay(newdata);
	m_displays[1].SetDisplay(&newdata[4]);
}


VoltageDisplay::VoltageDisplay()
{
	gpio addr0 = {A1_V0_GPIO_Port, A1_V0_Pin};
	gpio addr1 = {A0_V0_GPIO_Port, A0_V0_Pin};
	gpio wr1 = {WR_V0_GPIO_Port, WR_V0_Pin};
	gpio addr2 = {A0_V1_GPIO_Port, A0_V1_Pin};
	gpio addr3 = {A1_V1_GPIO_Port, A1_V1_Pin};
	gpio wr2 = {WR_V1_GPIO_Port, WR_V1_Pin};

	m_display.Init(addr0, addr1, wr1,addr2, addr3, wr2);
}

void VoltageDisplay::Clear()
{
	m_display.Set("        ");
}

void VoltageDisplay::Set(int32_t data)
{
	char tdata[8] = {0};
	uint32_t volts = data/1000;
	uint32_t milliVolts = data%1000;

	if(volts >= 10)
	{
		if(milliVolts >= 100)
		{
			milliVolts /= 10;		//less digits please
		}
		snprintf(tdata, 8, "%li.%02li V", volts, milliVolts);
	}
	else
	{
		snprintf(tdata, 8, "%li.%03li V", volts, milliVolts);
	}


	m_display.Set(tdata);
}

void VoltageDisplay::SetText(const char* data, int size)
{
	if(size <= Display::NUMBER_OF_DIGITS)
	{
		m_display.Set(data);
	}
}

int VoltageDisplay::LimitValue(bool range, int value)
{
	if(!range)
	{	//high range has double the voltage
		if(value >= MAXIMUM_VOLTAGE*2)
		{
			value = MAXIMUM_VOLTAGE*2;
		}
	}
	else
	{
		if(value >= MAXIMUM_VOLTAGE)
		{
			value = MAXIMUM_VOLTAGE;
		}
	}
	if(value < 0)
	{
		value = 0;
	}
	return value;
}

CurrentDisplay::CurrentDisplay()
{
	gpio addr0 = {A1_A0_GPIO_Port, A1_A0_Pin};
	gpio addr1 = {A0_A0_GPIO_Port, A0_A0_Pin};
	gpio wr1 = {WR_A0_GPIO_Port, WR_A0_Pin};
	gpio addr2 = {A0_A1_GPIO_Port, A0_A1_Pin};
	gpio addr3 = {A1_A1_GPIO_Port, A1_A1_Pin};
	gpio wr2 = {WR_A1_GPIO_Port, WR_A1_Pin};

	m_display.Init(addr0, addr1, wr1,addr2, addr3, wr2);

	m_value = 10; //Current needs an initial value of a little above 0
}

void CurrentDisplay::Clear()
{
	m_display.Set("        ");
}

void CurrentDisplay::Set(int32_t data)
{
	char tdata[8] = {0};
	uint32_t Amps = data/1000;
	uint32_t milliAmps = data%1000;

	snprintf(tdata, 8, "%li.%03li A", Amps, milliAmps);
	m_display.Set(tdata);
}

void CurrentDisplay::SetText(const char* data, int size)
{
	if(size <= Display::NUMBER_OF_DIGITS)
	{
		m_display.Set(data);
	}
}

int CurrentDisplay::LimitValue(bool range, int value)
{
	if(!range)
	{
		if(value >= MAXIMUM_CURRENT)
		{
			value = MAXIMUM_CURRENT;
		}
	}
	else
	{	//low range has double the current
		if(value >= MAXIMUM_CURRENT*2)
		{
			value = MAXIMUM_CURRENT*2;
		}
	}
	if(value < 0)
	{
		value = 0;
	}
	return value;
}


