/*
 * task.c
 *
 *  Created on: Nov 20, 2019
 *      Author: fidhi
 */
#include "task.h"
#include "stm32f1xx.h"
#include "UART.h"
#include "main.h"

unsigned char Button500(void);
unsigned char Button1000(void);
unsigned char ButtonProses(void);
unsigned char ButtonBatal(void);

#define LED500OFF	 				HAL_GPIO_WritePin(led0_GPIO_Port, led0_Pin, GPIO_PIN_SET)
#define LED500ON	 				HAL_GPIO_WritePin(led0_GPIO_Port, led0_Pin, GPIO_PIN_RESET)

#define LED1000OFF 					HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET)
#define LED1000ON	 				HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET)

#define LEDProsesOFF				HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET)
#define LEDProsesON					HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET)

#define LEDBatalOFF					HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_SET)
#define LEDBatalON 					HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET)

#define LEDKembaliOFF				HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET)
#define LEDKembaliON 				HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET)


//#define lamalampu 100
#define LAMA_LED500				500
#define LAMA_LED1000			1000
#define LAMA_LEDPROSES			6
#define LAMA_LED500_LED1000		3

_Bool delay=0;
_Bool delay1=0;
_Bool delay2=0;

#define Yes	1
#define No	0
enum state {ststart, st500, st1000, stkembalian500, stkembalian1000, stproses} keadaan;
volatile uint8_t debounce1=0xFF;
volatile uint8_t debounce2=0xFF;
volatile uint8_t debounce3=0xFF;
volatile uint8_t debounce4=0xFF;
volatile uint8_t button=0;
volatile uint16_t timeout;

void Task_Inisialisasi(void){
	keadaan=ststart;
}

void Task_Run(void){
	switch(keadaan){
	case ststart:
	{
		timeout=0;
		LED500OFF;
		LED1000OFF;
		LEDProsesOFF;
		LEDBatalOFF;
		LEDKembaliOFF;

		if(Button500()){
			UART_Print("Uang Anda Sudah Masuk \n\r");
			UART_Print("Saldo Anda = Rp500,- \n\r");
			keadaan=st500;

		}
		else if(Button1000()){
			UART_Print("Uang Anda Sudah Masuk \n\r");
			UART_Print("Saldo Anda = Rp1000,- \n\r");
			keadaan=stproses;
		}

		break;
	}

	case st500:
	{
		timeout=0;
		LED500ON;
		LED1000OFF;
		LEDProsesOFF;
		LEDBatalOFF;
		LEDKembaliOFF;
		//Plus500;
		if(Button500()){
			UART_Print("Saldo anda menjadi = Rp.1000,- \n\r");
			keadaan=stproses;
		}
		else if(Button1000()){
			UART_Print("Uang Kembalian Anda = Rp.500,- \n\r");
			keadaan=stkembalian500;

		}

		if(ButtonBatal()){
			//printf("Uang Keluar \n\r");
			UART_Print("Uang Keluar. Terimakasih. \n\r");
			keadaan=ststart;

		}
		break;
	}

	case st1000:
	{

		LED500OFF;
		LED1000OFF;
		LEDProsesON;
		LEDBatalOFF;
		LEDKembaliOFF;
		if(++timeout==LAMA_LED1000){
			//printf("Rp.1000 \n\r");
			keadaan=ststart;
			timeout=0;
		}

		break;
	}

	case stproses:
	{
		timeout=0;
		LED500OFF;
		LED1000ON;
		LEDProsesOFF;
		LEDBatalOFF;
		LEDKembaliOFF;

		if(Button500()){
			UART_Print("Uang Kembalian Anda = Rp.500,- \n\r");
			keadaan=stkembalian500;
		}
		else if(Button1000()){
			UART_Print("Uang Kembalian Anda = Rp.1000,- \n\r");
			keadaan=stkembalian1000;
		}

		if(ButtonBatal()){
			UART_Print("Uang Keluar. Terimakasih \n\r");
			keadaan=ststart;
		}

		if(ButtonProses()){
			UART_Print("Minuman Keluar \n\r");
			UART_Print("Selamat Menikmati \n\r");
			keadaan=st1000;
		}

		break;
	}

	case stkembalian500:
	{
		LED500OFF;
		LED1000OFF;
		LEDProsesOFF;
		LEDBatalON;
		LEDKembaliOFF;
		//printf("kembali Rp.500 \n\r");
		if(++timeout==LAMA_LED500){
			//printf("Rp.1000 \n\r");
			keadaan=stproses;
			timeout=0;
		}
		break;
	}

	case stkembalian1000:
	{
		LED500OFF;
		LED1000OFF;
		LEDProsesOFF;
		LEDBatalOFF;
		LEDKembaliON;
		//printf("kembali Rp.1000 \n\r");
		if(++timeout==LAMA_LED1000){
			//printf("Rp.1000 \n\r");
			keadaan=stproses;
			timeout=0;
		}
		break;
	}
}
}

unsigned char Button500(void){
	static unsigned char debounce1=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(pb1_GPIO_Port, pb1_Pin)== GPIO_PIN_RESET){
		debounce1=(debounce1<<1);
	} else {
		debounce1= (debounce1<<1)|1;
	}
	if (debounce1==0x07) {
		//printf(" \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}

unsigned char Button1000(void){
	static unsigned char debounce2=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(pb2_GPIO_Port, pb2_Pin)== GPIO_PIN_RESET){
		debounce2=(debounce2<<1);
	} else {
		debounce2= (debounce2<<1)|1;
	}
	if (debounce2==0x07) {
		//printf(" \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}

unsigned char ButtonProses(void){
	static unsigned char debounce3=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(pb3_GPIO_Port, pb3_Pin)== GPIO_PIN_RESET){
		debounce3=(debounce3<<1);
	} else {
		debounce3=(debounce3<<1)|1;
	}
	if (debounce3==0x07) {
		//printf(" \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}

unsigned char ButtonBatal(void){
	static unsigned char debounce4=0xFF;
	unsigned char detectedFLag=0;
	if(HAL_GPIO_ReadPin(pb4_GPIO_Port, pb4_Pin)== GPIO_PIN_RESET){
		debounce4=(debounce4<<1);
	} else {
		debounce4= (debounce4<<1)|1;
	}
	if (debounce4==0x07) {
		//printf(" \n\r");
		detectedFLag=1;
	}
	return detectedFLag;
}




