/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g0xx_hal.h"

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
#define BUCK_EN_Pin GPIO_PIN_9
#define BUCK_EN_GPIO_Port GPIOB
#define TRF_CURR_SENSE_Pin GPIO_PIN_0
#define TRF_CURR_SENSE_GPIO_Port GPIOA
#define CAP_VOLT_SENSE_Pin GPIO_PIN_1
#define CAP_VOLT_SENSE_GPIO_Port GPIOA
#define PUSHBUTTON_PIN_Pin GPIO_PIN_4
#define PUSHBUTTON_PIN_GPIO_Port GPIOA
#define VCAP_BUCK_CTRL_Pin GPIO_PIN_5
#define VCAP_BUCK_CTRL_GPIO_Port GPIOA
#define BAT_VOLT_SENSE_Pin GPIO_PIN_6
#define BAT_VOLT_SENSE_GPIO_Port GPIOA
#define LEVEL_POT_Pin GPIO_PIN_7
#define LEVEL_POT_GPIO_Port GPIOA
#define TRIAC_1_Pin GPIO_PIN_0
#define TRIAC_1_GPIO_Port GPIOB
#define TRIAC_2_Pin GPIO_PIN_1
#define TRIAC_2_GPIO_Port GPIOB
#define TRIAC_3_Pin GPIO_PIN_2
#define TRIAC_3_GPIO_Port GPIOB
#define Q1_Pin GPIO_PIN_8
#define Q1_GPIO_Port GPIOA
#define Q2_Pin GPIO_PIN_9
#define Q2_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_6
#define LED_1_GPIO_Port GPIOC
#define TRIAC_4_Pin GPIO_PIN_5
#define TRIAC_4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
