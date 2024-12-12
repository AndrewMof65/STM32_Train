/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "nucleo_hal_bsp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */




/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define WELCOME_MSG "\033[0;0H\033[2JWellcome to the Nucleo managemtnt console.\n\rSelect the optiom you are selected in:\n\r"
#define MAIN_MENU "\n\t1. Toggle LD2 LED\n\r\t2. Read USER BUTTON status.\n\r\t"
#define ADD_MENU1 "3. Make a blinking LD2 LED.\n\r\t4. Clear screen and print this message\n\r"
#define ADD_MENU2 "3. Stop a blinking LD2 LED.\n\r\t4. Clear screen and print this message\n\r"
#define PROMPT "\n\rВведите команду:"

#define DMA_TYPE 1
#define NO_TYPE 0
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
void printWellcomeMessage(uint8_t,uint8_t);
uint8_t readUserInput(void);
uint8_t processUserInput(uint8_t opt);

int main(void)
{
	uint8_t opt = 0;
	uint8_t status = 4;
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* USER CODE BEGIN Init */
  Nucleo_BSP_Init();
  /* USER CODE END Init */

  /* USER CODE BEGIN 2 */
  PrintMessage:
  	  printWellcomeMessage(DMA_TYPE,status);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  opt = readUserInput();
  	  status = processUserInput(opt);
  	  if(status > 2)
  		  goto PrintMessage;
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void ledBlink(void){
	while(1){
	    if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) {
	        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	        HAL_Delay(500);
	    }
	    else {
	    	return;
	    }
	}
}

void printWellcomeMessage(uint8_t transmitType,uint8_t status){
	int16_t len = strlen(WELCOME_MSG)+strlen(MAIN_MENU);
	len = (status == 3)? strlen(WELCOME_MSG)+strlen(MAIN_MENU)+strlen(ADD_MENU2):strlen(WELCOME_MSG)+strlen(MAIN_MENU)+strlen(ADD_MENU1);
	char msg[len];
	strcpy(msg,WELCOME_MSG);
	strcat(msg,MAIN_MENU);
	(status == 3)?strcat(msg,ADD_MENU2):strcat(msg,ADD_MENU1);
	switch(transmitType){
		default:
		case NO_TYPE:
			HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
			break;
		case DMA_TYPE:
			  HAL_DMA_Start(&hdma_usart2_tx,  (uint32_t)msg,  (uint32_t)&huart2.Instance->DR, strlen(msg));
			  //Enable UART in DMA mode
			  huart2.Instance->CR3 |= USART_CR3_DMAT;
			  //Wait for transfer complete
			  HAL_DMA_PollForTransfer(&hdma_usart2_tx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
			  //Disable UART DMA mode
			  huart2.Instance->CR3 &= ~USART_CR3_DMAT;
			break;
	}


}

uint8_t readUserInput(void){
	char readBuf[1];
	HAL_UART_Transmit(&huart2, (uint8_t*)PROMPT, strlen(PROMPT), HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, (uint8_t*)readBuf, 1, HAL_MAX_DELAY);
	return atoi(readBuf);
}

uint8_t processUserInput(uint8_t opt) {
	char msg[30];
	if(!opt || opt > 4)return 10;
	sprintf(msg, "%d", opt);
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
	switch(opt) {
		case 1:
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				break;
		case 2:
				sprintf(msg, "\r\nUSER BUTTON status: %s",
				HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET ? "PRESSED" : "RELEASED");
				HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
				break;
		case 3:
			if(HAL_TIM_Base_GetState(&htim6)==HAL_TIM_STATE_BUSY){
				HAL_TIM_Base_Stop_IT(&htim6);
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
				return 4;
			}
			else{
				HAL_TIM_Base_Start_IT(&htim6);
			}
		case 4:
			if(HAL_TIM_Base_GetState(&htim6)==HAL_TIM_STATE_BUSY)
				return 3;
		default:
			break;
	}
	return opt;
}

