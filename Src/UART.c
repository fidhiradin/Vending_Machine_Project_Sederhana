/*
 * UART.c
 *
 *  Created on: Nov 23, 2019
 *      Author: Piko Prasetyo
 */
#include "UART.h"


void UART_Print (char *pData){
	HAL_UART_Transmit(&huart1, (uint8_t *)pData, strlen(pData), 10);
}
