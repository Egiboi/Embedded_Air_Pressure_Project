/*
 * DigitalIoPin.cpp
 *
 *  Created on: 04.03.2020
 *      Author: Team 5
 */

#include "DigitalIoPin.h"

DigitalIoPin::DigitalIoPin(int port0, int pin0, bool input0, bool pullup0, bool invert0) {
	port = port0;
	pin = pin0;
	input = input0;
	pullup = pullup0;
	invert = invert0;

	if (input) {
		if (invert) {
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
		}

		else {
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
		}
	}

	else if (!input) {
		if (invert) {
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_INACT | IOCON_DIGMODE_EN | IOCON_INV_EN));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
		}
		else {
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_INACT | IOCON_DIGMODE_EN));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
		}
	}
}

DigitalIoPin::DigitalIoPin(const DigitalIoPin &p2) {
	port = p2.port;
	input = p2.input;
	pullup = p2.pullup;
	invert = p2.invert;

	Chip_IOCON_PinMuxSet(LPC_IOCON, p2.port, p2.pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	if (input) {

		Chip_GPIO_SetPinDIRInput(LPC_GPIO, p2.port, p2.pin);

	}

	else if (!input) {

		Chip_GPIO_SetPinDIROutput(LPC_GPIO, p2.port, p2.pin);

	}
}

DigitalIoPin::~DigitalIoPin() {

}

bool DigitalIoPin::read() { //READS THE STATE OF THE PIN. IF PIN = X TRUE, ELSE FALSE
	input = Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
	return input;
}

void DigitalIoPin::write(bool value) { //SETS THE STATE OF THE PIN
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, value);
}

void DigitalIoPin::setPortPin(int x, int y, bool z) {
	port = x;
	pin = y;
	invert = z;
}

void DigitalIoPin::setPortPinState(bool onoff) {
	if (onoff == true) {
		if (invert == true ) {
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, false);
		}
		else {
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, true);
		}
	}

	else {
		if (invert == true ) {
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, true);
		}
		else {
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, false);
		}
	}

}

