/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include "IntegerEdit.h"
#include <cstdio>

IntegerEdit::IntegerEdit(LiquidCrystal *lcd_, std::string editTitle, int min1, int max1): lcd(lcd_), title(editTitle) {
	value = 0;
	edit = 0;
	focus = false;
	min =min1;
	max=max1;
}

IntegerEdit::~IntegerEdit() {
}

void IntegerEdit::increment() {
	edit=(int)edit^max;
}

void IntegerEdit::decrement() {
	edit= (int)edit ^max;

}

void IntegerEdit::accept() {
	save();
}

void IntegerEdit::cancel() {
	edit = value;
}


void IntegerEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool IntegerEdit::getFocus() {
	return this->focus;
}

void IntegerEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0,1);
	char s[17];
	if(focus) {
		if(edit==1){
			snprintf(s, 17, "     [YES]     ");
		}else{
			snprintf(s, 17, "     [ NO]     ");
		}
	}
	else {
		if(edit==1){
			snprintf(s, 17, "      YES      ");
		}else{
			snprintf(s, 17, "       NO      ");
		}
	}
	lcd->print(s,2);
}


void IntegerEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
}


float IntegerEdit::getValue() {
	return value;
}
void IntegerEdit::setValue(float value) {
	edit = value;
	save();
}
bool IntegerEdit::getType(){
	return false;
}
