/*
 * TempSensor.cpp
 *
 *  Created on: May 22, 2021
 *      Author: rik
 */

#include "TempSensor.h"
#include "main.h"



TempSensor::TempSensor() {


}

int TempSensor::GetTemperature(I2C_HandleTypeDef* hi2c1)
{
	uint8_t data[1];
	data[0] = 0;
	HAL_I2C_Master_Transmit(hi2c1, address, data, 1, 100);
	HAL_I2C_Master_Receive(hi2c1, address, data, 1, 100);
	return data[0];
}
