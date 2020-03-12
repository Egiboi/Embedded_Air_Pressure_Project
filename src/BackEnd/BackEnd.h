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
	bool setFrequency(uint16_t freq);						//for setting fan speed
	bool readPressureSensor();								//for updating pressure sensor variable
	int getPressureSensor();								//prints pressure sensor variable
	void delayMicroseconds(unsigned int us);				//delay function
private:

	//modbus variables
	ModbusMaster node;
	ModbusRegister ControlWord;
	ModbusRegister StatusWord;
	ModbusRegister OutputFrequency;
	ModbusRegister Current;


	const uint16_t fa[21] = { 0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000 ,12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000 };

	//pressure sensor variables
	int16_t pressure;
};

#endif /* BACKEND_BACKEND_H_ */
