/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WR_A0_Pin GPIO_PIN_13
#define WR_A0_GPIO_Port GPIOC
#define A0_A0_Pin GPIO_PIN_14
#define A0_A0_GPIO_Port GPIOC
#define A1_A0_Pin GPIO_PIN_15
#define A1_A0_GPIO_Port GPIOC
#define A0_A1_Pin GPIO_PIN_0
#define A0_A1_GPIO_Port GPIOD
#define A1_A1_Pin GPIO_PIN_1
#define A1_A1_GPIO_Port GPIOD
#define D0_Pin GPIO_PIN_0
#define D0_GPIO_Port GPIOA
#define D1_Pin GPIO_PIN_1
#define D1_GPIO_Port GPIOA
#define D2_Pin GPIO_PIN_2
#define D2_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_3
#define D3_GPIO_Port GPIOA
#define D4_Pin GPIO_PIN_4
#define D4_GPIO_Port GPIOA
#define D5_Pin GPIO_PIN_5
#define D5_GPIO_Port GPIOA
#define D6_Pin GPIO_PIN_6
#define D6_GPIO_Port GPIOA
#define WR_V0_Pin GPIO_PIN_0
#define WR_V0_GPIO_Port GPIOB
#define A0_V0_Pin GPIO_PIN_1
#define A0_V0_GPIO_Port GPIOB
#define A1_V0_Pin GPIO_PIN_2
#define A1_V0_GPIO_Port GPIOB
#define WR_V1_Pin GPIO_PIN_10
#define WR_V1_GPIO_Port GPIOB
#define A0_V1_Pin GPIO_PIN_11
#define A0_V1_GPIO_Port GPIOB
#define A1_V1_Pin GPIO_PIN_12
#define A1_V1_GPIO_Port GPIOB
#define ROT_V_A_Pin GPIO_PIN_13
#define ROT_V_A_GPIO_Port GPIOB
#define ROT_V_A_EXTI_IRQn EXTI15_10_IRQn
#define ROT_V_B_Pin GPIO_PIN_14
#define ROT_V_B_GPIO_Port GPIOB
#define ROT_V_S_Pin GPIO_PIN_15
#define ROT_V_S_GPIO_Port GPIOB
#define ROT_V_S_EXTI_IRQn EXTI15_10_IRQn
#define ROT_A_S_Pin GPIO_PIN_8
#define ROT_A_S_GPIO_Port GPIOA
#define ROT_A_S_EXTI_IRQn EXTI9_5_IRQn
#define ROT_A_A_Pin GPIO_PIN_11
#define ROT_A_A_GPIO_Port GPIOA
#define ROT_A_A_EXTI_IRQn EXTI15_10_IRQn
#define ROT_A_B_Pin GPIO_PIN_12
#define ROT_A_B_GPIO_Port GPIOA
#define RELAY_Pin GPIO_PIN_8
#define RELAY_GPIO_Port GPIOB
#define WR_A1_Pin GPIO_PIN_9
#define WR_A1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
