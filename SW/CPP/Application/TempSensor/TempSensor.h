/*
 * TempSensor.h
 *
 *  Created on: May 22, 2021
 *      Author: rik
 */

#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include <stdint.h>
#include "main.h"

class TempSensor {
public:
	TempSensor();
	int GetTemperature(I2C_HandleTypeDef* hi2c1);
private:
	const uint8_t address = 0x4A << 1;
};

#endif /* TEMPSENSOR_H_ */
