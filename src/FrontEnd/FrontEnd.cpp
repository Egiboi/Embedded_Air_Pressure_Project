/*
 * FrontEnd.cpp
 *
 *  Created on: 12 Mar 2020
 *      Author: addei
 */

#include "FrontEnd.h"


FrontEnd::FrontEnd() {
	// TODO Auto-generated constructor stub

}

FrontEnd::~FrontEnd() {
	// TODO Auto-generated destructor stub
}

 /* namespace std */

/*int FrontEnd::modeSelect(int i){
	//int mode = frontend.modeSelect();
	if (mode != 0){
		manualset = false;
	}
}*/

uint16_t FrontEnd::defaultRun(uint16_t pre, uint16_t fsp){
	if (mode == 0){
		//modeSelect();
	}
	else if (mode == 1){
		automaticMode(pre, fsp);
	}
	else if (mode == 2){
		manualMode(fsp);
	}
}


uint16_t FrontEnd::manualMode(uint16_t freq){
	if (!manualset){
		//manualModeSetup();
	}
	else {


		return freq;
		//fanspeed = frontend.getFrequency();
		//backend.setFrequency(fanspeed);
	}
	// TODO frontend: print manualMode info to LCD
}

uint16_t FrontEnd::automaticMode(uint16_t currentpressure, uint16_t fanspeed){
	bool higherror = false;
	bool lowerror = false;


	if (pressuretarget > currentpressure){
		if (fanspeed <= fanspeedmax){
			return fanspeed++;
			//backend.setFrequency(++fanspeed);
			higherror = false;
		}
		else{
			higherror = true;
		}
	}
	else if (pressuretarget < currentpressure){
		if (fanspeed >= 0){
			//backend.setFrequency(--fanspeed);
			fanspeed--;
			lowerror = false;
		}
		else{
			lowerror = true;
		}
	}
	else{
		//backend.setFrequency(fanspeed);
		fanspeed;
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


