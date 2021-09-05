/*
 * RotaryEncoder.h
 *
 *  Created on: May 16, 2021
 *      Author: rik
 */

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include <stdint.h>
#include "main.h"



class RotaryEncoder {
public:
	/**
	 * @brief  Rotary encoder rotation status
	 */
	typedef enum {
		Rotate_Increment, /*!< Encoder was incremented */
		Rotate_Decrement, /*!< Encoder was decremented */
		Rotate_Nothing    /*!< Encoder stop at it was before */
	} Rotate_t;

	/**
	 * @brief  Rotary encoder mode selection for rotation
	 */
	typedef enum {
		Mode_Zero,        /*!< Rotary encoder mode zero. It is used for direction when it will be increment od decrement, default used */
		Mode_One          /*!< Rotary encoder mode one. It is used for direction when it will be increment od decrement */
	} Mode_t;

	int32_t Absolute;				/*!< Absolute rotation from beginning, for public use */
	int32_t Diff; 					/*!< Rotary difference from last check, for public use */
	Rotate_t Rotation;		/*!< Increment, Decrement or nothing, for public use */
	Mode_t Mode;              /*!< Rotary encoder mode selected */


	RotaryEncoder();

	void Init(GPIO_TypeDef* GPIO_A_Port, uint16_t GPIO_A_Pin, GPIO_TypeDef* GPIO_B_Port, uint16_t GPIO_B_Pin);
	void SetMode(Mode_t mode);
	Rotate_t Get();
	void Process();


private:
	uint8_t LastA;                  /*!< Last status of A pin when checking. Meant for private use */
	int32_t RE_Count;               /*!< Temporary variable to store data between rotation and user check */
	GPIO_TypeDef* GPIO_A;           /*!< Pointer to GPIOx for Rotary encode A pin. Meant for private use */
	GPIO_TypeDef* GPIO_B;           /*!< Pointer to GPIOx for Rotary encode B pin. Meant for private use */
	uint16_t GPIO_PIN_A;            /*!< GPIO pin for rotary encoder A pin. This pin is also set for interrupt */
	uint16_t GPIO_PIN_B;            /*!< GPIO pin for rotary encoder B pin. */
};

#endif /* ROTARYENCODER_H_ */
