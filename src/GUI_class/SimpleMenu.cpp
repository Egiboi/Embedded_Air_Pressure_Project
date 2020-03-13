/*
 * SimpleMenu.cpp
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#include "SimpleMenu.h"

#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>



SimpleMenu::SimpleMenu() {
	position = 0;
	ITM_init();
}

SimpleMenu::~SimpleMenu() {
	// TODO Auto-generated destructor stub
}

void SimpleMenu::addItem(MenuItem *item) {
	items.push_back(item);
}

void SimpleMenu::event(MenuItem::menuEvent e) {
	if(items.size() <= 0) return;

	if(!items[position]->event(e)) {
		if(e == MenuItem::up) position++;
		else if(e == MenuItem::down) position--;

		if(position < 0) position = items.size() - 1;
		if(position >= (int) items.size()) position = 0;

		items[position]->event(MenuItem::show);
	}
}
void SimpleMenu::print(){
	for (auto it = items.begin(); it != items.end(); it++) {
		char word[20]={};
		if((*it)->getType()){
			sprintf(word,"%.1f \n",(*it)->getValue());
		}else{
			sprintf(word,"%d \n",((int)(*it)->getValue()));
		}
	    ITM_write(word);
	}
}
