/*
 * modehandler.cpp
 *
 *  Created on: Mar 11, 2020
 *      Author: arge
 */

#include "BackEnd/BackEnd.h"

// TODO delet this clas :D
class FrontEnd {
public:
	FrontEnd(){}			// Placeholder for to-be-implemented frontend
	int modeSelect(){}		/* ModeSelect dialog, return 0 for cancelled, 1 for automatic and 2 for manual,
							this needs to be cancelled after 30 seconds */
	int getFrequency(){}	// get wanted frequency in manual mode dialog
	int getPressure(){}		// get wanted pressure in automatic mode dialog
	int setManualMode(){}	/* ManualModeSetup dialog, for setting initial fan speed,
							* return 0 for cancelled, 1 for automatic and 2 for manual,
							this needs to be cancelled after 30 seconds */
};

BackEnd backend;
FrontEnd frontend;

int mode = 0;	// Setting mode to 0 will redirect flow to modeSelect()
int fanspeedmax = 20;
int fanspeed = 10;
bool manualset = false;

void defaultRun();
void manualModeSetup();
void manualMode();
void automaticMode();

void modeSelect(){
	int mode = frontend.modeSelect();
	if (mode != 0){
		manualset = false;
	}
}

void defaultRun(){
	if (mode == 0){
		modeSelect();
	}
	else if (mode == 1){
		automaticMode();
	}
	else if (mode == 2){
		manualMode();
	}
}

void manualModeSetup(){
	int oldmode = mode;
	mode = frontend.setManualMode();
	if (mode == 0) mode = oldmode;
	else{
		fanspeed = frontend.getFrequency();
		manualset = true;
	}
}

void manualMode(){
	if (!manualset){
		manualModeSetup();
	}
	else{
		fanspeed = frontend.getFrequency();
		backend.setFrequency(fanspeed);
	}
	// TODO frontend: print manualMode info to LCD
}

void automaticMode(){
	bool higherror = false;
	bool lowerror = false;
	int wantedpressure = frontend.getPressure();
	int currentpressure = backend.getPressureSensor();

	if (wantedpressure < currentpressure){
		if (fanspeed <= fanspeedmax){
			backend.setFrequency(++fanspeed);
			higherror = false;
		}
		else{
			higherror = true;
		}
	}
	else if (wantedpressure > currentpressure){
		if (fanspeed >= 0){
			backend.setFrequency(--fanspeed);
			lowerror = false;
		}
		else{
			lowerror = true;
		}
	}
	else{
		backend.setFrequency(fanspeed);
	}
	if (higherror || lowerror){
		if (higherror){
			// TODO frontend: print error info (fan at max and cant go higher) to LCD
		}
		if (lowerror){
			// TODO frontend: print error info (fan at 0 and cant go lower) to LCD
		}
		higherror = false;
		lowerror = false;
	}
	else{
		// TODO frontend: print automaticMode info to the LCD
	}
}

void mainloop(){
	while (mode == 0){
		mode = modeSelect();
	}
	while(1){
		defaultRun();
		/* defaultRun() needs to be interrupted at button press
		 * to adjust the fan speeds and mode selections etc. which
		 * will be used at the next loop
		 */
	}
}












