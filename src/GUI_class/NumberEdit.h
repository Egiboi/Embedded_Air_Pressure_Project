/*
 * DecimalEdit.h
 *
 *  Created on: 25.2.2020
 *      Author: danie
 */

#include "BooleanEdit.h"


class NumberEdit:public BooleanEdit {
public:
	NumberEdit(LiquidCrystal *lcd_, std::string editTitle, int min, int max,float increment);
	virtual ~NumberEdit();
	void increment()override;
	void decrement()override;
	void display()override;
	float getValue()override;
	bool getType();
private:
	float incr;
};

