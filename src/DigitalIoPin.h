/*
 * DigitalIoPin.h
 *
 *  Created on: 04.03.2020
 *      Author: Team 5
 */

#include "board.h"

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_

class DigitalIoPin {
	public:
		DigitalIoPin(int port = 0, int pin = 0, bool input = true, bool pullup = true, bool invert = false);
		DigitalIoPin(const DigitalIoPin &p2);
		virtual ~DigitalIoPin();
		bool read();
		void write(bool value);
		void setPortPin(int x, int y, bool z);
		void setPortPinState(bool onoff);

	private:     // add these as needed }
		int port;
		int pin;
		bool input;
		bool pullup;
		bool invert;
};

#endif /* DIGITALIOPI_H_ */
