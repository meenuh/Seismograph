/*
 * SD_Read_Write.hpp
 *
 *  Created on: Dec 12, 2016
 *      Author: andre
 */

#ifndef L5_APPLICATION_SD_READ_WRITE_HPP_
#define L5_APPLICATION_SD_READ_WRITE_HPP_
#include "tasks.hpp"
#include <stdio.h>
#include <LPC17xx.h> //for memory mapped values
#define max 1000

class SD_Read_Write: public scheduler_task {
public:
	SD_Read_Write(uint8_t priority);
	bool init();
	bool run(void *p);
private:
char buffer[max] = {0};
};




#endif /* L5_APPLICATION_SD_READ_WRITE_HPP_ */
