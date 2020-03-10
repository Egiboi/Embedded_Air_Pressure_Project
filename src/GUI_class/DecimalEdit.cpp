/*
 * DecimalEdit.cpp
 *
 *  Created on: 25.2.2020
 *      Author: danie
 */

#include "DecimalEdit.h"
#include <cstdio>

DecimalEdit::DecimalEdit(LiquidCrystal *lcd_, std::string editTitle, int min1, int max1,float increment):IntegerEdit(lcd_,editTitle,min1,max1){
	incr= increment;
}
void DecimalEdit::increment(){
	if(edit+incr<=(max+incr*9/10)){
		edit= edit+ incr;
	}

}
void DecimalEdit::decrement(){
	if(edit-incr>=min){
		edit=edit-incr;
	}
}
DecimalEdit::~DecimalEdit(){

}
void DecimalEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0,1);
	char s[17];
	if(focus) {
		snprintf(s, 17, "     [%4.1f]     ", edit);
	}
	else {
		snprintf(s, 17, "      %4.1f      ", edit);
	}
	lcd->print(s,2);
}
float DecimalEdit::getValue() {
	return value;
}
bool DecimalEdit::getType(){
	return true;
}

