/*
 * PCControl.h
 *
 *  Created on: Jun 15, 2021
 *      Author: rik
 */

#ifndef PCCONTROL_H_
#define PCCONTROL_H_

#include "main.h"
#include <stdint.h>
#include <array>

class IPCControl {
public:
	virtual void LockControlToPC() = 0;
	virtual void UnlockControlToPC()= 0;
	virtual bool SetMilliVolt(uint32_t milliVolt) = 0;
	virtual bool SetMilliAmps(uint32_t milliAmps) = 0;
	virtual uint32_t GetMilliVolt() = 0;
	virtual uint32_t GetMilliAmps() = 0;
	virtual uint32_t GetTemperatureInCelcius() = 0;
	virtual ~IPCControl() = default;

};

class PCControl {
public:
	PCControl();
	void Init(IPCControl* PCControlCallbacks);
	void Run();
	void ProcessUartInterrupt(UART_HandleTypeDef *huart);

private:
	enum class Command_e
	{
		GET_MV,
		GET_MA,
		SET_MV,
		SET_MA,
		GET_TEMP,
		GET_CTRL,
		DROP_CTRL,
		INVALID,
	};
	struct IssuedCommand_t
	{
		Command_e command;
		uint32_t value;
	};

	std::array<uint8_t, 64> m_uartBuffer;
	uint8_t m_uartCounter = 0;
	IssuedCommand_t m_issuedCommand = {Command_e::INVALID, 0};
	IPCControl* m_callbacks = nullptr;

	IssuedCommand_t ParseUartCommand(char* data, uint8_t size);
	void ClearUartReceiveBuffer();
	uint32_t GetValueFromUartCommand(char* data, uint8_t size);
	void ResponseSendOK();
	void ResponseSendNOK();

	//to deal with the hal function wanting a uint8_t*
	HAL_StatusTypeDef UART_TRANSMIT(UART_HandleTypeDef *huart, const char *pData, uint16_t Size, uint32_t Timeout);


};

#endif /* PCCONTROL_H_ */
