/*
 * Analog_To_Digital.cpp
 *
 *  Created on: Nov 13, 2016
 *      Author: andre
 */

#include "Analog_To_Digital.hpp"
#include "printf_lib.h"

#include <string.h>
#include "storage.hpp"

static uint32_t time = 0;

Analog_To_Digital::Analog_To_Digital(uint8_t priority) :
		scheduler_task("A2D", 4096, priority) {
}

bool Analog_To_Digital::init() {
	LPC_SC->PCONP |= (1 << 12);		// Note: Clear the PDN bit in the AD0CR before clearing this bit, and set this bit before setting PDN

	LPC_ADC->ADCR |= (1 << 21);  	// Setting the A/D converter to be operational
	LPC_ADC->ADCR &= ~(0xFF<<0);
	LPC_ADC->ADCR |= (1 << 5); 		// Setting Analog pin 5 to be turned on port 1 pin 31

	//LPC_ADC->ADCR |= (1 << 4);
	//LPC_ADC->ADCR |= (1 << 3);

	LPC_ADC->ADCR |= (10 << 8);		// Setting the PCLOCK (I just set this to a random value, I am not entirely sure what we need)

	LPC_SC->PCLKSEL0 |= (3 << 24); 	// Setting the pclock for the A/D converter

	LPC_PINCON->PINSEL3 &= ~(3 << 30);	// Clearing the PINSEL regiter
	LPC_PINCON->PINSEL3 |= (3 << 30); 	// Setting port 1 pin 31 to be analog to digital 5 turned on

	//LPC_ADC->ADCR |= (1 << 24);			// This starts the A/D converter to start

	QueueHandle_t seismographData = xQueueCreate(25, sizeof(DATA));
	addSharedObject(sharedQueue_ID, seismographData);

	SemaphoreHandle_t mutexHandle;
	mutexHandle = xSemaphoreCreateMutex();
	addSharedObject(sharedMutex_ID, mutexHandle);

	SemaphoreHandle_t binaryHandle = xSemaphoreCreateBinary();
	addSharedObject(sharedBinary_ID, binaryHandle);

	return true;
}

bool Analog_To_Digital::run(void *p) {
	DATA data;

	LPC_ADC->ADCR |= (1 << 24);			// This starts the A/D converter to start
	while((LPC_ADC->ADGDR & (1 << 31)) == 0);

	adcResults = (LPC_ADC->ADGDR >> 4) & 0xFFF;
	if(adcResults > 0xB00){
		xSemaphoreGive(getSharedObject(sharedBinary_ID));
	}
	data.adcValue = adcResults >> 4;
	data.time = time;

//	xQueueSend(getSharedObject(sharedQueue_ID), &data, portMAX_DELAY); // sends the value in the queue

	sprintf(buffer, "%i, %x\n", time, adcResults);

//	xSemaphoreTake(getSharedObject(sharedBinary_ID));

	if(xSemaphoreTake(getSharedObject(sharedMutex_ID), 1000)){
		//char *ptr;
		Storage::append("1:data.txt", buffer, strlen(buffer), SEEK_SET);

		xQueueSend(getSharedObject(sharedQueue_ID), &data, portMAX_DELAY);
		//u0_dbg_printf("reading from card: %s\n", buffer);
		xSemaphoreGive(getSharedObject(sharedMutex_ID));
	}

	time++;

	vTaskDelay(200);
	return true;
}
