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
	bool readPressureSensor();
	int getPressureSensor();
private:
	int pressure;
	static const int current;
	const uint16_t fa[20] = { 1000, 2000, 3000, 3500, 4000, 5000, 7000, 8000, 10000, 15000, 20000, 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000, 1000 };

};

#endif /* BACKEND_BACKEND_H_ */
