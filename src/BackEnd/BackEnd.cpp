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
	//Config I2C
	I2C_config conf;
	I2C presSens(conf);
	//Initalize data container
	uint8_t data[3]={};
	int16_t dataCombined = 0, mask=0x00FFFFFF;
	//Command to read device through I2C
	uint8_t commandRead = 0xF1;
	//Generator polynomial for CRC-8 check (data sheet 5/10)
	uint16_t CRC = 0x131;

	uint32_t CRCdata =0;
	bool I2CCheck=true;

	//Write 1 byte to I2C, with commandRead
	if(!presSens.write(I2C_PRES_ADDR, &commandRead, 1)){
		//printf("I2C write fail\n");
		I2CCheck=false;
	}
	//Read 3 bytes of reply, 2 data, 1 CRC
	else if(!presSens.read(I2C_PRES_ADDR, data, 3)){
		//printf("I2C read fail\n");
		I2CCheck=false;
	}else{
		//Combine the 2 data bytes to one 16bit int
		dataCombined= data[0];
		dataCombined = dataCombined << 8;
		dataCombined |= data[1];

		//Combine all 3 bytes to one 32 bit int
		CRCdata |= dataCombined;
		CRCdata = CRCdata << 8;
		CRCdata |= data[2];
		CRCdata &= mask;
		//loop lenght 2 bytes (2nd and 3rd bit)
		int k = 16+8;

		while (k-(8-1) != 0) {
			if ((CRCdata & (1 << k)) >> k == 1) {
				int tempk = k;
				for (int n = 8; n >= 0; n--) {
					//CRCdata bit
					int bit1 = (CRCdata & (1 << tempk)) >> tempk;
					//Generator Polynomial bit
					int bit2 = (CRC & (1 << n)) >> n;

					//set bit if xor between bit1 and bit2 true, else unset
					if (bit1 ^ bit2) {
						CRCdata |= 1UL<<tempk;
					}else{
						CRCdata &= ~(1UL << tempk);
					}
					tempk--;
				}
			}
			k--;
		}
	}

	if (CRCdata == 0 && I2CCheck ==TRUE) {
		//altitude fix
		dataCombined = dataCombined *0.95/240;
		//printf("%d\n", (int)dataCombined);
		pressure = dataCombined;
		return true;
	}
	else if(I2CCheck && CRCdata!=0) {
		//printf("CRC check failed\n");
		return false;
	}

	else if(!I2CCheck && CRCdata==0){
		//Cannot happen
		//printf("I2C transaction failed\n");
		return false;
	}

	else{
		//printf("Multifail\n");
		return false;
	}
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



