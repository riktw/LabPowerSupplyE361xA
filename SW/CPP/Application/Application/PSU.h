/*
 * PSU.h
 *
 *  Created on: May 22, 2021
 *      Author: rik
 */

#ifndef PSU_H_
#define PSU_H_

#include "../TempSensor/TempSensor.h"
#include "../AnalogControl/AnalogControl.h"

class PSU {
public:

	PSU();
	void Update();
	int GetVoltage();
	void SetVoltage(int voltage);
	int GetCurrent();
	void SetCurrent(int current);
	int GetTemperature();
	void SetRange(bool range);

private:
	enum class state
	{
		Idle,
		WaitForVoltage,
		WaitForCurrent,
		GetTemperature,
	};

	int m_readVoltage, m_setVoltage, m_readCurrent, m_setCurrent = 0;
	int m_temperature = 0;
	state m_state = state::Idle;

	TempSensor m_tempSensor;
	AnalogControl m_analogControl;
};

#endif /* PSU_H_ */
