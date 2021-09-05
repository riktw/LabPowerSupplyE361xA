/*
 * HMI.h
 *
 *  Created on: May 18, 2021
 *      Author: rik
 */

#ifndef HMI_H_
#define HMI_H_

#include "../Display/HPDL1414.h"
#include "../RotaryEncoder/RotaryEncoder.h"

class HMI {
public:
	HMI();

	void ShowSplashScreen();
	void ProcessGpioInterrupt(uint16_t GPIO_Pin);
	void SetMeasuredVoltage(int voltage);
	void SetMeasuredCurrent(int current);
	void SetMeasuredTemperature(int temperature);
	int GetSetVoltage();
	int GetSetCurrent();
	bool GetRange();
	void SetPcControl(bool PCControl);
	void SetOverTemperature(bool overTemperature);
	void Update();

private:
	enum class range {LOW, HIGH};

	int HandleRotaryEncoder(RotaryEncoder& encoder);
	void HandleDisplay(int dataDiff, int measuredValue, BaseDisplay& display, bool overTemperature);

	VoltageDisplay m_voltageDisplay;
	CurrentDisplay m_currentDisplay;

	RotaryEncoder m_voltPot;
	RotaryEncoder m_currentPot;

	//TODO: are tracked in two places now, remove this?
	bool m_PCControl = false;
	bool m_overTemperature = false;
	bool m_showTemp = false;

	bool m_rotaryAmpPressed = false;

	int m_measuredVoltage, m_measuredCurrent, m_measuredTemperature = 0;

	uint32_t m_counter, m_showTemperatureCounter = 0;

	range m_range = range::HIGH;
};

#endif /* HMI_H_ */
