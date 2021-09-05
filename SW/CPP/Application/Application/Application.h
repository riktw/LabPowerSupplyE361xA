/*
 * Wrapper.h
 *
 *  Created on: May 12, 2021
 *      Author: rik
 */

#ifndef APPLICATION_WRAPPER_WRAPPER_H_
#define APPLICATION_WRAPPER_WRAPPER_H_

#include "HMI.h"
#include "PSU.h"
#include "PCControl.h"
#include "main.h"

#include <stdint.h>
#include <array>

class Application: public IPCControl {
public:
	Application();

	void Run();
	void ProcessGpioInterrupt(uint16_t GPIO_Pin);
	void ProcessUartInterrupt(UART_HandleTypeDef *huart);

private:
	HMI m_HMI;
	PSU m_PSU;
	PCControl m_PCControl;

	bool m_PCLockedControl = false;
	bool m_overTemperature = false;
	static constexpr uint8_t MAX_TEMPERATURE = 65;
	static constexpr uint8_t TEMPERATURE_HYST = 5;

	void LockControlToPC() override;
	void UnlockControlToPC() override;
	bool SetMilliVolt(uint32_t milliVolt) override;
	bool SetMilliAmps(uint32_t milliAmps) override;
	uint32_t GetMilliVolt() override;
	uint32_t GetMilliAmps() override;
	uint32_t GetTemperatureInCelcius() override;


};

#endif /* APPLICATION_WRAPPER_WRAPPER_H_ */

