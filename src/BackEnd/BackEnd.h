/*
 * BackEnd.h
 *
 *  Created on: 10 Mar 2020
 *      Author: addei
 */

#ifndef BACKEND_BACKEND_H_
#define BACKEND_BACKEND_H_

#include "../ModbusMaster/ModbusMaster.h"
#include "../ModbusMaster/ModbusRegister.h"
#include "../ModbusMaster/LpcUart.h"
#include "../i2c_class/I2C.h"

/** 7-bit I2C addresses of  sensor (data sheet page 4/10) */
#define I2C_PRES_ADDR  (0x40)

class BackEnd {
public:
	BackEnd();
	virtual ~BackEnd();
	bool setFrequency(uint16_t fanspeed);						//for setting fan speed
	bool readPressureSensor();								//for updating pressure sensor variable
	int getPressureSensor();								//prints pressure sensor variable
	void delayMicroseconds(unsigned int us);				//delay function
	void setPinInterrupt(int port, int pin, int interruptChannel); //sets pint interrupts 0-2
	void prepPinForSet (int port, int pin); 				//Preps pins for interrupting
	bool insertAndCheckCircBuf(int index, int value);		//Tried making ringbuffer for button reading, didn't have time to work
	uint16_t getFrequency();
private:

	//modbus variables
	ModbusMaster node;
	ModbusRegister ControlWord;
	ModbusRegister StatusWord;
	ModbusRegister OutputFrequency;
	ModbusRegister Current;
	I2C_config conf;
	int circBuf1 [10], circBuf2[10], circBuf3[10];//unused
	uint16_t speed;

	const uint16_t fa[21] = { 0, 1500, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000 ,12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000 };

	//pressure sensor variable
	uint16_t pressure;
};

#endif /* BACKEND_BACKEND_H_ */
