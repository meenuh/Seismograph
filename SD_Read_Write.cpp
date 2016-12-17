/*
 * SD_Read_Write.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: andre
 */
#include "SD_Read_Write.hpp"
#include "Analog_To_Digital.hpp"
#include "storage.hpp"
#include "printf_lib.h"
#include "soft_timer.hpp"
#include <time.h>
#include <string.h>
SD_Read_Write::SD_Read_Write(uint8_t priority) :
		scheduler_task("SD", 2048, priority) {

}


bool SD_Read_Write::init(){
	return true;
}

bool SD_Read_Write::run(void *p){
	QueueHandle_t qid = getSharedObject(sharedQueue_ID);
	uint16_t data;
	static uint8_t time = 0;

	if(xQueueReceive(qid, &data, portMAX_DELAY)){
		sprintf(buffer, "%i, %x \r\n", time, data);
			//if((time % 2) == 0){ //write every 1s
				printf("The time function is real\n)");
			Storage::append("1:data.txt", buffer, strlen(buffer), SEEK_SET);
			//memset(buffer,0,max); 	// resets the buffer to 0
			//}
			printf("The value of time is%i\n",time);
		time++;
	}
	return true;
}
