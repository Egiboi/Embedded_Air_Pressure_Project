/*
 * FrontEnd.cpp
 *
 *  Created on: 12 Mar 2020
 *      Author: addei
 */

#include "FrontEnd.h"


FrontEnd::FrontEnd() {
	// TODO Auto-generated constructor stub
	errorcode=0;

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

int FrontEnd::getMode(){
	return mode;
}

uint16_t FrontEnd::defaultRun(uint16_t pre, uint16_t fsp) {
	if (mode == 0){
		//modeSelect();
	}
	else if (mode == 1){
		return automaticMode(pre, fsp);

	}
	else if (mode == 2){
		return manualMode(fsp);
	}
}


uint16_t FrontEnd::manualMode(uint16_t freq) {
	return freq;
}

uint16_t FrontEnd::automaticMode(uint16_t currentpressure, uint16_t fanspeed) {
	bool higherror = false;
	bool lowerror = false;


	if (pressuretarget > currentpressure){
		if (fanspeed <= fanspeedmax){
			return ++fanspeed;
			higherror = false;
		}
		else{
			higherror = true;
		}
	}
	else if (pressuretarget < currentpressure){
		if (fanspeed >= 0){
			return --fanspeed;
			lowerror = false;
		}
		else{
			lowerror = true;
		}
	}
	else {
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
	else {
		// TODO frontend: print automaticMode info to the LCD
	}
}

void FrontEnd::setPressureTarget(uint16_t trg) {
	pressuretarget = trg;
}

void FrontEnd::setMode(int i) {
	mode = i;
}

void FrontEnd::defaultDisplay(LiquidCrystal*lcd, int fanspeed, int pascal){
	lcd->clear();
	lcd->setCursor(0,0);
	char s1[17],s2[17];

	if(mode==1) {
		snprintf(s1, 17, "Pascals:%3d AUTO", pascal);
	}
	else if (mode==2) {
		snprintf(s1, 17, "Pascals:%3d MANU", pascal);
	}else if(mode==0){
		snprintf(s1, 17,"Unset mode");
	}
	lcd->print(s1,1);
	if(errorcode!=0){
		snprintf(s2, 17, "Speed:%2d/20 FAIL",fanspeed);

	}else{
		snprintf(s2, 17, "Speed:%2d/20     ",fanspeed);
	}
	lcd->setCursor(0,1);
	lcd->print(s2,2);
}


