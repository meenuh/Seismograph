/*
 * Joystick.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: dearm
 */
#include "Joystick.hpp"

Joystick::Joystick(uint8_t priority) : scheduler_task("joystick", 2048, priority) {

}

bool Joystick::run(void *p){
	uint16_t adcResults, second;

	LPC_ADC->ADCR |= (1 << 24);

	while((LPC_ADC->ADDR4 & (1 << 31)) == 0);

	second = (LPC_ADC->ADDR4 >> 4) & 0xFFF;
	second = second >> 2;

	  if (second < 50){
		  u0_dbg_printf("left\n");
	  }else if(second < 650){
		  u0_dbg_printf("Right\n");
	  }

	u0_dbg_printf("result from joystick %x\n", second);
	vTaskDelay(1000);

	return true;
}

bool Joystick::init() {

	//set pin 1.30 to ADC functionality
	LPC_PINCON->PINSEL3 &= ~(3 << 28);
	LPC_PINCON->PINSEL3 |= (3 << 28);

	return true;
}

