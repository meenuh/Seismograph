/*
 * HTC_Bluetooth.cpp
 *
 *  Created on: Dec 10, 2016
 *      Author: andre
 */
#include "HTC_Bluetooth.hpp"
#include "printf_lib.h"
HTC_Bluetooth::HTC_Bluetooth(uint8_t priority) :
		scheduler_task("Bluetooth", 2048, priority) {
}

void HTC_Bluetooth::UART2_putChar(char out) {
	LPC_UART2->THR = out;
	while (1) {
		if (LPC_UART2->LSR & (1 << 6)) {
			break;
		}
	}
}
void HTC_Bluetooth::UART2_putByte(int out) {
	LPC_UART2->THR = out;
	while (1) {
		if (LPC_UART2->LSR & (1 << 6)) {
			break;
		}
	}
}

char HTC_Bluetooth::UART2_getChar(void){
	while (1) {
		if (LPC_UART2->LSR & (1 << 0)) {
			break;
		}
	}
		char byte = LPC_UART2->RBR;
		return byte;
}

/**
 * 2.8 - tx
 * 2.9 - rx
 */
bool HTC_Bluetooth::init(){
	// This sets my Baud Rate to 9600
		uint32_t Div = sys_get_cpu_clock() / ((16 * 9600));

		//This actives UART3
		LPC_SC->PCONP |= (1 << 24); // This is setting the PCONP register to turn on UART2 peripheral

		//This is setting the peripheral  clock
		LPC_SC->PCLKSEL1 &= ~(3 << 16);	// This is  clearing any bits previously  that are set
		LPC_SC->PCLKSEL1 |= (1 << 16);// This value sets the peripheral clock to be exactly the same as the main clock

		//This is setting the line control  register
		LPC_UART2->LCR &= ~(0xFF << 0);	// This is clearing the entire line control register
		LPC_UART2->LCR |= (1 << 7);	// This is setting DLAB = 1 and allowing for a 8 bit word transfer

		LPC_UART2->FCR &= ~(0xFF << 0);	// This is clearing the entire FCR Register
		LPC_UART2->FCR |= (1 << 0);	// This is enabling the fifo when using the UART

		LPC_UART2->DLL = Div;// This is setting the DLL register to be equal to my speicifc baud rate
		LPC_UART2->DLM = (Div >> 8);// This is setting the DLM and shifting the baud rate to the right by 8 bits
		LPC_UART2->LCR = 3;	// This is setting the transfer to be 8 bits wide.

		// select pin 2.8 - tx2
		LPC_PINCON->PINSEL4 &= ~(3 << 0);
		LPC_PINCON->PINSEL4 |= (2 << 16);

		// select pin 2.9 - rx2
		LPC_PINCON->PINSEL4 &= ~(3 << 18);
		LPC_PINCON->PINSEL4 |= (2 << 18);
return true;
}


bool HTC_Bluetooth::run(void *p){
	//printf("I am here\n");
	UART2_putChar('\n');
	UART2_putChar('A');
	UART2_putChar('T');
	UART2_putChar('H');
	UART2_putChar('i');
	u0_dbg_printf("The first letter is:%c",UART2_getChar());
//	u0_dbg_printf("The second letter is:%c",UART2_getChar());
	//vTaskDelay(1000);
	return true;

}
