/*
 * BackEnd.cpp
 *
 *  Created on: 10 Mar 2020
 *      Author: addei
 */

#include "BackEnd.h"

BackEnd::BackEnd() {
	// TODO Auto-generated constructor stub
	node = ModbusMaster(2); // Create modbus object that connects to slave id 2
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	ControlWord = ModbusRegister(&node, 0);
	StatusWord = ModbusRegister(&node, 3);
	OutputFrequency = ModbusRegister(&node, 102);
	Current = ModbusRegister(&node, 103);
	delayMicroseconds(1000);
	ControlWord = 0x0406; // prepare for starting
	delayMicroseconds(1000);
	ControlWord = 0x047F; // set drive to start mode
	delayMicroseconds(1000);
}

BackEnd::~BackEnd() {
	// TODO Auto-generated destructor stub
}

bool BackEnd::setFrequency(uint16_t freq)
{
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	ModbusRegister Frequency(&node, 1); // reference 1
	ModbusRegister StatusWord(&node, 3);

	Frequency = freq; // set motor frequency

	//printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		//Sleep(delay);
		delayMicroseconds(delay);
		// read status word
		result = StatusWord;
		// check if we are at setpoint
		if (result >= 0 && (result & 0x0100)) atSetpoint = true;
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	//printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}

bool BackEnd::readPressureSensor() {


	return false;
}



int BackEnd::getPressureSensor() {

	return pressure;
}

void BackEnd::delayMicroseconds(unsigned int us)
{
	uint64_t ticks = (Chip_Clock_GetSystemClockRate()/1000000)*us;
	Chip_RIT_ClearCTRL(LPC_RITIMER,0x08);
	Chip_RIT_SetCompareValue(LPC_RITIMER,ticks);
	Chip_RIT_SetCounter(LPC_RITIMER,0);
	Chip_RIT_SetCTRL(LPC_RITIMER,0x08);
	while((LPC_RITIMER->CTRL & 0x1)==0);
	Chip_RIT_ClearCTRL(LPC_RITIMER,0x08);
	Chip_RIT_ClearCTRL(LPC_RITIMER,0x01);
}



