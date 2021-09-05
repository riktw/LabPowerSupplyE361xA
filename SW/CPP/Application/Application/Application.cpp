/*
 * Application.cpp
 * Small wrapper to go from C to C++
 *
 *  Created on: May 12, 2021
 *      Author: rik
 */

#include "../Application/Application.h"

#include "../Application/start.h"


namespace
{
Application wrapper;
}

void start(void)
{
	wrapper.Run();
}

Application::Application()
{

}


void Application::Run()
{
	m_PCControl.Init(this);

	m_HMI.ShowSplashScreen();
	HAL_Delay(1000);

	while(1)
	{
		m_HMI.Update();
		m_PSU.Update();

		if(m_PSU.GetTemperature() >= MAX_TEMPERATURE)
		{
			m_overTemperature = true;
			m_HMI.SetOverTemperature(m_overTemperature);
		}

		if(m_overTemperature)
		{
			if(m_PSU.GetTemperature() <= (MAX_TEMPERATURE - TEMPERATURE_HYST))
			{
				m_overTemperature = false;
				m_HMI.SetOverTemperature(m_overTemperature);
			}
			else
			{
				m_PSU.SetVoltage(10);
				m_PSU.SetCurrent(10);
			}
		}
		else
		{
			m_PCControl.Run();
			m_HMI.SetMeasuredVoltage(m_PSU.GetVoltage());
			m_HMI.SetMeasuredCurrent(m_PSU.GetCurrent());
			m_HMI.SetMeasuredTemperature(m_PSU.GetTemperature());
			if(!m_PCLockedControl)
			{
				m_PSU.SetVoltage(m_HMI.GetSetVoltage());
				m_PSU.SetCurrent(m_HMI.GetSetCurrent());
				m_PSU.SetRange(m_HMI.GetRange());
			}
		}
		HAL_Delay(100);
	}
}

void Application::LockControlToPC()
{
	m_PCLockedControl = true;
	m_HMI.SetPcControl(m_PCLockedControl);
}

void Application::UnlockControlToPC()
{
	m_PCLockedControl = false;
	m_HMI.SetPcControl(m_PCLockedControl);
}

bool Application::SetMilliVolt(uint32_t milliVolt)
{
	if(m_PCLockedControl)
	{
		m_PSU.SetVoltage(milliVolt);
	}
	return m_PCLockedControl;
}

bool Application::SetMilliAmps(uint32_t milliAmps)
{
	if(m_PCLockedControl)
	{
		m_PSU.SetCurrent(milliAmps);
	}
	return m_PCLockedControl;
}

uint32_t Application::GetMilliVolt()
{
	return m_PSU.GetVoltage();
}

uint32_t Application::GetMilliAmps()
{
	return m_PSU.GetCurrent();
}

uint32_t Application::GetTemperatureInCelcius()
{
	return m_PSU.GetTemperature();
}



void Application::ProcessGpioInterrupt(uint16_t GPIO_Pin)
{
	m_HMI.ProcessGpioInterrupt(GPIO_Pin);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	wrapper.ProcessGpioInterrupt(GPIO_Pin);
}

void Application::ProcessUartInterrupt(UART_HandleTypeDef *huart)
{
	m_PCControl.ProcessUartInterrupt(huart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	wrapper.ProcessUartInterrupt(huart);
}
