/*
 * nucleo_hal_bsp.h
 *
 *  Created on: Dec 11, 2024
 *      Author: afomenko
 */

#ifndef INC_NUCLEO_HAL_BSP_H_
#define INC_NUCLEO_HAL_BSP_H_


/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern TIM_HandleTypeDef htim6;

void Nucleo_BSP_Init() ;



#endif /* INC_NUCLEO_HAL_BSP_H_ */
