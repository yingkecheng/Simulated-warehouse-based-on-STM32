/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void motor_toggle(void)
{
	static unsigned char stat = 1;
	HAL_GPIO_WritePin(A_SLP_GPIO_Port, A_SLP_Pin, stat);
	HAL_GPIO_WritePin(A_EN_GPIO_Port, A_EN_Pin, stat);
	HAL_GPIO_WritePin(A_PH_GPIO_Port, A_PH_Pin, GPIO_PIN_SET);
	stat ^= 1;
}


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )

{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
	static int count = 0;
	static unsigned char old_key0 = 1;
	static unsigned char old_key1 = 1;
	static unsigned char old_key2 = 1;
	unsigned char new_key0;
	unsigned char new_key1;
	unsigned char new_key2;
	
	count++;
	
	if ((count % 10) == 0)
	{
		new_key0 = HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);
		new_key1 = HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
		new_key2 = HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
		
		if (old_key0 && !new_key0)
		{
			motor_toggle();
		}
		
		if (old_key1 && !new_key1)
		{
			motor_toggle();
		}
		
		if (old_key2 && !new_key2)
		{
			motor_toggle();
		}
		
		old_key0 = new_key0;
		old_key1 = new_key1;
		old_key2 = new_key2;
	}
	
	if ((count % 1000) == 0)
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}
}
/* USER CODE END 3 */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

