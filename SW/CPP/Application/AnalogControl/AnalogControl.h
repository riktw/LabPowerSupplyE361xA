/*
 * AnalogControl.h
 *
 *  Created on: May 22, 2021
 *      Author: rik
 */

#ifndef ANALOGCONTROL_H_
#define ANALOGCONTROL_H_


#include <stdint.h>
#include "main.h"

class AnalogControl {
public:
	AnalogControl();
	int GetCurrentAndStartVoltageMeasurement(I2C_HandleTypeDef* hi2c1);
	int GetVoltageAndStartCurrentMeasurement(I2C_HandleTypeDef* hi2c1);
	void SetCurrent(int current, I2C_HandleTypeDef* hi2c1);
	void SetVoltage(int voltage, I2C_HandleTypeDef* hi2c1);

private:
	const uint8_t m_DACAddress = 0x41 << 1;
	const uint8_t m_ADCAddress = 0x34 << 1;

	const uint8_t ADC_VOLT_CHANNEL = 0xB9;
	const uint8_t ADC_AMPS_CHANNEL = 0xB1;
	const uint8_t ADC_MODE_GAIN1 = 0x80;

	const uint8_t DAC_VOLT_CHANNEL = 0x30;
	const uint8_t DAC_AMPS_CHANNEL = 0x31;

	void ClearAllI2CErrors(I2C_HandleTypeDef* hi2c1);
};

#endif /* ANALOGCONTROL_H_ */
