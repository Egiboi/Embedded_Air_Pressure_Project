/*
 * DecimalEdit.cpp
 *
 *  Created on: 25.2.2020
 *      Author: danie
 */

#include "NumberEdit.h"

#include <cstdio>

NumberEdit::NumberEdit(LiquidCrystal *lcd_, std::string editTitle, int min1, int max1,float increment):BooleanEdit(lcd_,editTitle,min1,max1){
	incr= increment;
}
void NumberEdit::increment(){
	if(edit+incr<=(max+incr*9/10)){
		edit= edit+ incr;
	}

}
void NumberEdit::decrement(){
	if(edit-incr>=min){
		edit=edit-incr;
	}
}
NumberEdit::~NumberEdit(){

}
void NumberEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0,1);
	char s[17];
	if(focus) {
		snprintf(s, 17, "     [%3.0f]     ", edit);
	}
	else {
		snprintf(s, 17, "      %3.0f      ", edit);
	}
	lcd->print(s,2);
}
float NumberEdit::getValue() {
	return value;
}
bool NumberEdit::getType(){
	return true;
}

