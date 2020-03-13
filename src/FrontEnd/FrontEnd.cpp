/*
 * FrontEnd.cpp
 *
 *  Created on: 12 Mar 2020
 *      Author: addei
 */

#include "FrontEnd.h"


FrontEnd::FrontEnd() {
	errorcode=0;
	looper=0;
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
	}else{
		return 0;
	}
}


uint16_t FrontEnd::manualMode(uint16_t freq) {
	return freq;
}

uint16_t FrontEnd::automaticMode(uint16_t currentpressure, uint16_t fanspeed) {
	bool higherror = false;
	bool lowerror = false;


	if (pressuretarget > currentpressure){
		//High speed
		if (fanspeed < fanspeedmax){
			higherror = false;
			looper=0;
			errorcode=0;
			return ++fanspeed;
		}
		else {
			higherror = true;
			++looper;
		}
	}
	else if (pressuretarget < currentpressure){
		//Low speed
		if (fanspeed > 0) {
			lowerror = false;
			looper=0;
			errorcode=0;
			return --fanspeed;
		}
		else{
			//Low error currently not reachable as fan stops at zero
			lowerror = true;
			++looper;

		}
	}
	else {
		//Good Speed
		return fanspeed;
	}
	//Loops 20 times after max/min speed reached, then error made set
	if (looper>20) {
		if (higherror){
			errorcode = 2;
		}
		if (lowerror){
			errorcode = 1;

		}

		higherror = false;
		lowerror = false;

	}
	else {
		errorcode = 0;
	}
	//Fan speed already at max/min at this point return current so fan doesn't stop
	//Reaches here only when target pressure out of reach
	return fanspeed;
}

void FrontEnd::setPressureTarget(uint16_t trg) {
	pressuretarget = trg;
}

void FrontEnd::setMode(int i) {
	mode = i;
	errorcode=0;
}

void FrontEnd::defaultDisplay(LiquidCrystal*lcd, int fanspeed, int pascal){
	lcd->clear();
	lcd->setCursor(0,0);
	char s1[17],s2[17];
	//firstline
	if(mode==1) {
		snprintf(s1, 17, "Pascals:%3d AUTO", pascal);
	}
	else if (mode==2) {
		snprintf(s1, 17, "Pascals:%3d MANU", pascal);
	}else if(mode==0){
		snprintf(s1, 17,"Standby mode");
	}
	//Second line
	lcd->print(s1,1);
	if(errorcode!=0){
		snprintf(s2, 17, "Speed:%2d/20 FAIL",fanspeed);

	}else{
		snprintf(s2, 17, "Speed:%2d/20     ",fanspeed);
	}
	lcd->setCursor(0,1);
	lcd->print(s2,2);
}

int FrontEnd::getErrorCode() {
	return errorcode;
}


