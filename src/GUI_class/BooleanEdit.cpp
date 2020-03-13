/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include "BooleanEdit.h"

#include <cstdio>

BooleanEdit::BooleanEdit(LiquidCrystal *lcd_, std::string editTitle, int min1, int max1): lcd(lcd_), title(editTitle) {
	value = 0;
	edit = 0;
	focus = false;
	min =min1;
	max=max1;
}

BooleanEdit::~BooleanEdit() {
}

void BooleanEdit::increment() {
	edit=(int)edit^max;
}

void BooleanEdit::decrement() {
	edit= (int)edit ^max;

}

void BooleanEdit::accept() {
	save();
}

void BooleanEdit::cancel() {
	edit = value;
}


void BooleanEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool BooleanEdit::getFocus() {
	return this->focus;
}

void BooleanEdit::display() {
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


void BooleanEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
}


float BooleanEdit::getValue() {
	return value;
}
void BooleanEdit::setValue(float value) {
	edit = value;
	save();
}
bool BooleanEdit::getType(){
	return false;
}
