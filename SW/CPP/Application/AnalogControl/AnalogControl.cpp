/*
 * AnalogControl.cpp
 *
 *  Created on: May 22, 2021
 *      Author: rik
 */

#include "AnalogControl.h"

AnalogControl::AnalogControl()
{

}

int AnalogControl::GetCurrentAndStartVoltageMeasurement(I2C_HandleTypeDef* hi2c1)
{
	uint16_t newSettings = (ADC_VOLT_CHANNEL << 8) + ADC_MODE_GAIN1;
	uint8_t data[3];
	if(HAL_I2C_Mem_Read(hi2c1, m_ADCAddress, newSettings, I2C_MEMADD_SIZE_16BIT, data, 3, 100) != HAL_OK)
	{
		ClearAllI2CErrors(hi2c1);
	}

	int currentRead = (data[0] - 0x80) << 16;
	currentRead += data[1] << 8;
	currentRead += data[0];
	currentRead = currentRead >> 6;

	currentRead = currentRead / 45;
	currentRead -= 10;
	if(currentRead < 0)
	{
		currentRead = 0;
	}

	return currentRead;
}


int AnalogControl::GetVoltageAndStartCurrentMeasurement(I2C_HandleTypeDef* hi2c1)
{
	uint16_t newSettings = (ADC_AMPS_CHANNEL << 8) + (ADC_MODE_GAIN1);
	uint8_t data[3];
	if(HAL_I2C_Mem_Read(hi2c1, m_ADCAddress, newSettings, I2C_MEMADD_SIZE_16BIT, data, 3, 100) != HAL_OK)
	{
		ClearAllI2CErrors(hi2c1);
	}

	int voltageRead = (data[0] - 0x80) << 16;
	voltageRead += data[1] << 8;
	voltageRead += data[0];
	voltageRead = voltageRead >> 6; // last 6 bit are always 0, shift to get the correct value.

	//voltage is 32768 for 2.5V measured.
	voltageRead = voltageRead / 26; //scale to mv.
	int voltageAfterCorrection = (5000 - (voltageRead*2));
	int currentR19 = (voltageAfterCorrection*10) / 47;
	int voltagemv = currentR19 * 39;
	voltagemv = voltagemv - (voltageRead*2);
	return voltagemv;
}


void AnalogControl::SetCurrent(int current, I2C_HandleTypeDef* hi2c1)
{
	uint8_t data[3];
	data[0] = DAC_AMPS_CHANNEL;

	int currentRaw = (current*255*100) / 591; //Convert to 16 bit value for DAC.
	data[1] = currentRaw >> 8;
	data[2] = currentRaw & 0xFF;

	if(HAL_I2C_Master_Transmit(hi2c1, m_DACAddress, data, 3, 100) != HAL_OK)
	{
		ClearAllI2CErrors(hi2c1);
	}
}

void AnalogControl::SetVoltage(int voltage, I2C_HandleTypeDef* hi2c1)
{
	uint8_t data[3];
	data[0] = DAC_VOLT_CHANNEL;

	int voltageRaw = (voltage*255) / 115; //Convert to 16 bit value for DAC.
	data[1] = voltageRaw >> 8;
	data[2] = voltageRaw & 0xFF;

	if(HAL_I2C_Master_Transmit(hi2c1, m_DACAddress, data, 3, 100) != HAL_OK)
	{
		ClearAllI2CErrors(hi2c1);
	}
}

void AnalogControl::ClearAllI2CErrors(I2C_HandleTypeDef* hi2c1)
{	//
	HAL_I2C_DeInit(hi2c1);
	HAL_I2C_Init(hi2c1);
}
