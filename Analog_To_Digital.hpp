/*
 * Analog_To_Digital.hpp
 *
 *  Created on: Nov 13, 2016
 *      Author: andre
 */

#ifndef L5_APPLICATION_ANALOG_TO_DIGITAL_HPP_
#define L5_APPLICATION_ANALOG_TO_DIGITAL_HPP_

#include "tasks.hpp"
#include <stdio.h>
#include <LPC17xx.h> //for memory mapped values

//SemaphoreHandle_t gatekeeper;

typedef enum {
	sharedQueue_ID,	// This is the shared id between both tasks
	sharedMutex_ID,
	sharedBinary_ID
} sharedHandleId_t;

typedef struct data{
	uint32_t time;
	uint16_t adcValue;
}DATA;

#define MAX 50

class Analog_To_Digital: public scheduler_task {
public:
	Analog_To_Digital(uint8_t priority);
	bool init();
//	void saveData();
	bool run(void *p);
private:
	uint16_t adcResults;
	char buffer[MAX] = {0};
};

#endif /* L5_APPLICATION_ANALOG_TO_DIGITAL_HPP_ */
