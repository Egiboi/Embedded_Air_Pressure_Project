/*
 * FrontEnd.h
 *
 *  Created on: 12 Mar 2020
 *      Author: addei
 */

#ifndef FRONTEND_FRONTEND_H_
#define FRONTEND_FRONTEND_H_

#include <stdint.h>
#include "../GUI_class/LiquidCrystal.h"

class FrontEnd {
public:
	FrontEnd();
	virtual ~FrontEnd();
	//int modeSelect(int i);			/* ModeSelect dialog, return 0 for cancelled, 1 for automatic and 2 for manual, this needs to be cancelled after 30 seconds */
	//int getFrequency(uint16_t i);	// get wanted frequency in manual mode dialog
	//int getPressure(uint16_t i);	// get wanted pressure in automatic mode dialog
	int setMode();					/* ManualModeSetup dialog, for setting initial fan speed, this needs to be cancelled after 30 seconds */
	uint16_t defaultRun(uint16_t pre, uint16_t fsp);
	void setPressureTarget(uint16_t trg);
	void setMode(int i);
	void defaultDisplay(LiquidCrystal*lcd, int fanspeed, int pascal);
	int getMode();
private:
	int mode = 0;					// Setting mode to 0 will redirect flow to modeSelect(), auto = 1, manual = 2.
	uint16_t pressuretarget = 0;	// Target for the automaticMode
	int errorcode;

	bool manualset = false;
	uint16_t manualMode(uint16_t freq);
	uint16_t automaticMode(uint16_t currentpressure, uint16_t fanspeed);
	const uint16_t fanspeedmax = 20;

};


#endif /* FRONTEND_FRONTEND_H_ */
