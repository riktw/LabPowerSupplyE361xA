/*
 * RotaryEncoder.cpp
 *
 *  Created on: May 16, 2021
 *      Author: rik
 *      Based on: https://stm32f4-discovery.net/2014/08/library-26-rotary-encoder-stm32f4/
 */

#include "RotaryEncoder.h"



RotaryEncoder::RotaryEncoder() {


}

void RotaryEncoder::Init(GPIO_TypeDef* GPIO_A_Port, uint16_t GPIO_A_Pin, GPIO_TypeDef* GPIO_B_Port, uint16_t GPIO_B_Pin)
{
	/* Save parameters */
	GPIO_A = GPIO_A_Port;
	GPIO_B = GPIO_B_Port;
	GPIO_PIN_A = GPIO_A_Pin;
	GPIO_PIN_B = GPIO_B_Pin;

	/* Set default mode */
	Mode = Mode_Zero;

	/* Set default */
	RE_Count = 0;
	Diff = 0;
	Absolute = 0;
	LastA = 1;
}

RotaryEncoder::Rotate_t RotaryEncoder::Get()
{
	/* Calculate everything */
	Diff = RE_Count - Absolute;
	Absolute = RE_Count;

	/* Check */
	if (RE_Count < 0)
	{
		Rotation = Rotate_Decrement;
		return Rotate_Decrement;
	}
	else if (RE_Count > 0)
	{
		Rotation = Rotate_Increment;
		return Rotate_Increment;
	}

	Rotation = Rotate_Nothing;
	return Rotate_Nothing;
}

void RotaryEncoder::SetMode(Mode_t mode)
{
	/* Set mode */
	Mode = mode;
}

void RotaryEncoder::Process()
{
	uint8_t now_a;
	uint8_t now_b;

	/* Read inputs */
	now_a = HAL_GPIO_ReadPin(GPIO_A, GPIO_PIN_A);
	now_b = HAL_GPIO_ReadPin(GPIO_B, GPIO_PIN_B);

	/* Check difference */
	if (now_a != LastA)
	{
		LastA = now_a;

		if (LastA == 0)
		{
			/* Check mode */
			if (Mode == Mode_Zero)
			{
				if (now_b == 1)
				{
					RE_Count--;
				} else
				{
					RE_Count++;
				}
			}
			else
			{
				if (now_b == 1)
				{
					RE_Count++;
				}
				else
				{
					RE_Count--;
				}
			}
		}
	}
}
