/*
 * HMI.cpp
 *
 *  Created on: May 18, 2021
 *      Author: rik
 */

#include "HMI.h"
#include <cstdio>

HMI::HMI()
{
	m_voltPot.Init(ROT_V_A_GPIO_Port, ROT_V_A_Pin, ROT_V_B_GPIO_Port, ROT_V_B_Pin);
	m_currentPot.Init(ROT_A_A_GPIO_Port, ROT_A_A_Pin, ROT_A_B_GPIO_Port, ROT_A_B_Pin);
	m_voltPot.SetMode(RotaryEncoder::Mode_t::Mode_One);
	m_currentPot.SetMode(RotaryEncoder::Mode_t::Mode_One);
}

void HMI::ShowSplashScreen()
{
	m_voltageDisplay.SetText("JAEBLOG ", 8);
	m_currentDisplay.SetText("LAB PSU ", 8);
}

void HMI::Update()
{
	enum class menu_e {NONE = 0, ENTERING, RANGE, TEMP, EXIT};

	static int menuCounter = 0;
	static menu_e inMenu = menu_e::NONE;


	if(m_rotaryAmpPressed && inMenu == menu_e::NONE)
	{
		if(!HAL_GPIO_ReadPin(ROT_A_S_GPIO_Port, ROT_A_S_Pin))
		{
			menuCounter++;
			if(menuCounter >= 10)
			{
				inMenu = menu_e::ENTERING;
				menuCounter = 0;
				m_currentDisplay.m_valueChanged = false;
				m_rotaryAmpPressed = false;
			}
		}
		else
		{
			menuCounter = 0;
			m_currentDisplay.m_valueChanged = false;
			m_rotaryAmpPressed = false;
		}
	}

	if(m_PCControl)
	{
		m_voltageDisplay.SetText("PC      ", 8);
		m_currentDisplay.SetText("CONTROL ", 8);
	}
	else if(inMenu != menu_e::NONE)
	{
		//TODO: menu handling to separate function!
		inMenu = static_cast<menu_e>(static_cast<int>(inMenu) + HandleRotaryEncoder(m_currentPot));
		static bool rangeChanged = false;
		static range tempRange = m_range;

		switch (inMenu)
		{
		case menu_e::ENTERING:
			inMenu = menu_e::RANGE;
			if(tempRange == range::HIGH)
			{
				m_voltageDisplay.SetText("30V - 1A", 8);
			}
			else
			{
				m_voltageDisplay.SetText("15V - 2A", 8);
			}
			m_currentDisplay.SetText("RANGE   ", 8);
			break;
		case menu_e::RANGE:
			if(tempRange == range::HIGH)
			{
				m_voltageDisplay.SetText("30V - 1A", 8);
			}
			else
			{
				m_voltageDisplay.SetText("15V - 2A", 8);
			}
			m_currentDisplay.SetText("RANGE   ", 8);
			break;
		case menu_e::TEMP:
			char temp[8];
			snprintf(temp, sizeof(temp), "TEMP:%i", m_measuredTemperature);
			m_currentDisplay.SetText("        ", 8);
			m_voltageDisplay.SetText(temp, 8);
			break;
		case menu_e::EXIT:
			m_voltageDisplay.SetText("        ", 8);
			m_currentDisplay.SetText("EXIT    ", 8);
			break;
		default:
			if(static_cast<int>(inMenu) <= 0)
			{
				inMenu = menu_e::RANGE;
			}
			else
			{
				inMenu = menu_e::EXIT;
			}
			break;
		}

		if(m_rotaryAmpPressed)
		{
			m_rotaryAmpPressed = false;
			if(inMenu == menu_e::TEMP || inMenu == menu_e::EXIT)
			{
				inMenu = menu_e::NONE;

				if(rangeChanged)
				{
					rangeChanged = false;
					m_range = tempRange;
					m_voltageDisplay.m_value = m_voltageDisplay.LimitValue(GetRange(), m_voltageDisplay.m_value);
					m_currentDisplay.m_value = m_currentDisplay.LimitValue(GetRange(), m_currentDisplay.m_value);
				}
			}
			else
			{
				rangeChanged = true;
				if(tempRange == range::HIGH)
				{
					tempRange = range::LOW;
				}
				else
				{
					tempRange = range::HIGH;
				}
			}
		}

	}
	else
	{
		int voltDiff = HandleRotaryEncoder(m_voltPot) * 100;
		int ampDiff = HandleRotaryEncoder(m_currentPot) * 10;

		if(m_showTemp)
		{

		}
		else
		{
			HandleDisplay(voltDiff, m_measuredVoltage, m_voltageDisplay, m_overTemperature);
		}
		HandleDisplay(ampDiff, m_measuredCurrent, m_currentDisplay, m_overTemperature);
	}

	m_counter++;
	if(m_showTemperatureCounter+10 <= m_counter)
	{
		m_showTemp = false;
	}
}


void HMI::SetMeasuredVoltage(int voltage)
{
	m_measuredVoltage = voltage;
}

void HMI::SetMeasuredCurrent(int current)
{
	m_measuredCurrent = current;
}

void HMI::SetMeasuredTemperature(int temperature)
{
	m_measuredTemperature = temperature;
}


int HMI::GetSetVoltage()
{
	if(m_voltageDisplay.m_valueChanged)
	{
		return m_voltageDisplay.m_oldValue;
	}
	else
	{
		return m_voltageDisplay.m_value;
	}
}

int HMI::GetSetCurrent()
{
	if(m_currentDisplay.m_valueChanged)
	{
		return m_currentDisplay.m_oldValue;
	}
	else
	{
		return m_currentDisplay.m_value;
	}
}

bool HMI::GetRange()
{
	return (m_range == range::HIGH) ? false : true;
}

void HMI::SetPcControl(bool PCControl)
{
	m_PCControl = PCControl;
}

void HMI::SetOverTemperature(bool overTemperature)
{
	m_overTemperature = overTemperature;
}

void HMI::ProcessGpioInterrupt(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == ROT_V_A_Pin)
	{
		m_voltPot.Process();
	}
	else if(GPIO_Pin == ROT_A_A_Pin)
	{
		m_currentPot.Process();
	}
	else if(GPIO_Pin == ROT_V_S_Pin)
	{
		m_voltageDisplay.m_valueChanged = false;
	}
	else if(GPIO_Pin == ROT_A_S_Pin)
	{
		m_rotaryAmpPressed = true;
	}
}

int HMI::HandleRotaryEncoder(RotaryEncoder& encoder)
{
	int newValue = 0;

	encoder.Get();

	if(encoder.Diff >= 3  || encoder.Diff <= -3)
	{	//when encoder is turned fast, change values even faster for simple acceleration
		newValue = encoder.Diff * 3;
	}
	else
	{
		newValue = encoder.Diff * 1;
	}
	return newValue;
}

void HMI::HandleDisplay(int dataDiff, int measuredValue,  BaseDisplay& display, bool overTemperature)
{
	if(dataDiff != 0)
	{
		if(!display.m_valueChanged)
		{
			display.m_oldValue = display.m_value;
			display.m_valueChanged = true;
		}
		display.m_value += dataDiff;
		display.m_value = display.LimitValue(GetRange(), display.m_value);
		display.m_valueChangedCounter = 0;
	}

	if(display.m_valueChanged)
	{
		if(display.m_valueChangedCounter > 30)
		{
			display.m_value = display.m_oldValue;
			display.m_valueChanged = false;
		}
		display.m_valueChangedCounter++;
		if(m_counter%2)
		{
			display.Set(display.m_value);
		}
		else
		{
			display.Clear();
		}
	}
	else
	{
		if(overTemperature)
		{
			display.SetText("OVERTEMP", 8);
		}
		else
		{
			display.Set(measuredValue);
		}
	}
}
