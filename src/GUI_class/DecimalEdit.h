/*
 * DecimalEdit.h
 *
 *  Created on: 25.2.2020
 *      Author: danie
 */

#include "IntegerEdit.h"


class DecimalEdit:public IntegerEdit {
public:
	DecimalEdit(LiquidCrystal *lcd_, std::string editTitle, int min, int max,float increment);
	virtual ~DecimalEdit();
	void increment()override;
	void decrement()override;
	void display()override;
	float getValue()override;
	bool getType();
private:
	float incr;
};

