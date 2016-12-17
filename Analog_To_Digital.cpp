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

static unsigned int time = 0;

Analog_To_Digital::Analog_To_Digital(uint8_t priority) :
		scheduler_task("A2D", 2048, priority) {
}

bool Analog_To_Digital::init() {
	LPC_SC->PCONP |= (1 << 12);		// Note: Clear the PDN bit in the AD0CR before clearing this bit, and set this bit before setting PDN

	LPC_ADC->ADCR |= (1 << 21);  	// Setting the A/D converter to be operational
	LPC_ADC->ADCR &= ~(0xFF<<0);
	LPC_ADC->ADCR |= (1 << 5); 		// Setting Analog pin 5 to be turned on port 1 pin 31
	LPC_ADC->ADCR |= (10 << 8);		// Setting the PCLOCK (I just set this to a random value, I am not entirely sure what we need)

	LPC_SC->PCLKSEL0 |= (3 << 24); 	// Setting the pclock for the A/D converter

	LPC_PINCON->PINSEL3 &= ~(3 << 30);	// Clearing the PINSEL regiter
	LPC_PINCON->PINSEL3 |= (3 << 30); 	// Setting port 1 pin 31 to be analog to digital 5 turned on

	//LPC_ADC->ADCR |= (1 << 24);			// This starts the A/D converter to start

	// This is where we intantiate our QUEUE

	QueueHandle_t seismographData = xQueueCreate(25,sizeof(DATA));
	addSharedObject(sharedQueue_ID,seismographData);
	return true;
}

/*void Analog_To_Digital::saveData(){
	QueueHandle_t qid = getSharedObject(sharedQueue_ID);
	uint16_t data;

	if(xQueueReceive(qid, &data, portMAX_DELAY)){
		sprintf(buffer, "%i, %x \r\n", time, data);
		//Storage::append("1:data.txt", buffer, strlen(buffer), SEEK_SET);
		printf("The value of time is%i\n",time);
		time++;
	}
}*/

bool Analog_To_Digital::run(void *p) {
//	uint16_t data;
	DATA data;
	static uint16_t adcSum = 0;

	LPC_ADC->ADCR |= (1 << 24);			// This starts the A/D converter to start
	while((LPC_ADC->ADGDR & (1 << 31)) == 0);

	adcResults = (LPC_ADC->ADGDR >> 4) & 0xFFF;
	data.adcValue = adcResults >> 4;
	data.time = time;
	//adcSum += (adcResults >> 4);
		xQueueSend(getSharedObject(sharedQueue_ID),&data,1000); // sends the value in the queue

	//data.adcValue = (adcResults >> 4);
	//data.time = time;
	//adcResults = (adcResults >> 4);
	//sprintf(buffer, "%i, %x\n", time, adcResults);
	//Storage::append("1:data.txt", buffer, strlen(buffer), SEEK_SET);
	//printf("The value of time is%i\n",time);
	time++;
	//u0_dbg_printf("The results of the adc is %x\n", adcResults);
	vTaskDelay(500);
	return true;
}
