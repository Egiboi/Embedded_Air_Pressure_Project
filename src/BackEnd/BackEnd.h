/*
 * BackEnd.h
 *
 *  Created on: 10 Mar 2020
 *      Author: addei
 */

#ifndef BACKEND_BACKEND_H_
#define BACKEND_BACKEND_H_

class BackEnd {
public:
	BackEnd();
	virtual ~BackEnd();
	bool setFrequency(ModbusMaster& node, uint16_t freq);
private:
	ModbusRegister asd; //TODO väliaikainen, korjaa OIKEAAN MUOTOON
	ModbusMaster ass; //TODO väliaikainen, korjaa muotoon referenssi
};

#endif /* BACKEND_BACKEND_H_ */
