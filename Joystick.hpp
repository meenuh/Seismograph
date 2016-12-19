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
#include "tasks.hpp"

class Joystick : public scheduler_task {
public:
	Joystick(uint8_t priority);

	bool run(void *p);

	bool init();
};

#endif /* L5_APPLICATION_SOURCE_JOYSTICK_HPP_ */
