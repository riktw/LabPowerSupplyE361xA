/*
 * PSU.cpp
 *
 *  Created on: May 22, 2021
 *      Author: rik
 */

#include "PSU.h"
#include "main.h"
extern I2C_HandleTypeDef hi2c1;

PSU::PSU()
{

}

//needs to be called every 100ms
void PSU::Update()
{
	static int counter = 0;
	counter++;
	switch (m_state) {
	case state::Idle:
		m_analogControl.SetCurrent(0, &hi2c1);
		m_analogControl.SetVoltage(0, &hi2c1);
		m_analogControl.GetCurrentAndStartVoltageMeasurement(&hi2c1);
		m_state = state::WaitForVoltage;
		counter = 0;
		break;
	case state::WaitForVoltage:
		if(counter == 2)
		{
			m_readVoltage = m_analogControl.GetVoltageAndStartCurrentMeasurement(&hi2c1);
			m_state = state::WaitForCurrent;
			counter = 0;
		}
		break;
	case state::WaitForCurrent:
		if(counter == 2)
		{
			m_readCurrent = m_analogControl.GetCurrentAndStartVoltageMeasurement(&hi2c1);
			m_state = state::GetTemperature;
			counter = 0;
		}
		break;
	case state::GetTemperature:
		m_temperature = m_tempSensor.GetTemperature(&hi2c1);
		m_state = state::WaitForVoltage;
		counter = 0;
	default:
		break;
	}
}

int PSU::GetVoltage()
{
	return m_readVoltage;
}

void PSU::SetVoltage(int voltage)
{
	m_analogControl.SetVoltage(voltage, &hi2c1);
}

int PSU::GetCurrent()
{
	return m_readCurrent;
}

void PSU::SetCurrent(int current)
{
	m_analogControl.SetCurrent(current, &hi2c1);
}

int PSU::GetTemperature()
{
	return m_temperature;
}

void PSU::SetRange(bool range)
{
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, static_cast<GPIO_PinState>(range));
}

