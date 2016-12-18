/*
 * Joystick.hpp
 *
 *  Created on: Dec 17, 2016
 *      Author: dearm
 */

#ifndef L5_APPLICATION_SOURCE_JOYSTICK_HPP_
#define L5_APPLICATION_SOURCE_JOYSTICK_HPP_

#include <stdio.h>
#include <LPC17xx.h> //for memory mapped values
#include "printf_lib.h"

class Joystick : public scheduler_task {
public:
	Joystick(uint8_t priority) : scheduler_task("joystick", 2048, priority) {

	}

	bool run(void *p){
		uint16_t adcResults, second;

		while((LPC_ADC->ADDR4 & (1 << 31)) == 0);

		second = (LPC_ADC->ADDR4 >> 4) & 0xFFF;
		second = second >> 2;

		  if (second < 50){
			  u0_dbg_printf("left\n");
		  }else if(second < 650){
			  u0_dbg_printf("Right\n");
		  }

		//u0_dbg_printf("result from joystick %x\n", second);
		//vTaskDelay(1000);

		return true;
	}

	bool init() {

		//set pin 1.30 to ADC functionality
		LPC_PINCON->PINSEL3 &= ~(3 << 28);
		LPC_PINCON->PINSEL3 |= (3 << 28);

		return true;
	}
};

#endif /* L5_APPLICATION_SOURCE_JOYSTICK_HPP_ */
