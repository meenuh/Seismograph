/*
 * HTC_Bluetooth.hpp
 *
 *  Created on: Dec 10, 2016
 *      Author: andre
 */

#ifndef L5_APPLICATION_HTC_BLUETOOTH_HPP_
#define L5_APPLICATION_HTC_BLUETOOTH_HPP_

#include "tasks.hpp"
#include <stdio.h>
#include <LPC17xx.h> //for memory mapped values

#include "Analog_To_Digital.hpp"

class HTC_Bluetooth: public scheduler_task {
public:
	HTC_Bluetooth(uint8_t priority);
	bool init();
	bool run(void *p);
	void UART2_putChar(char out);
	void UART2_putByte(int out);
	char UART2_getChar(void);
	int UART2_getByte(void);
private:
};

#endif /* L5_APPLICATION_HTC_BLUETOOTH_HPP_ */
