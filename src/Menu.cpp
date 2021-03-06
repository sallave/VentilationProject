/*
 * Menu.cpp
 *
 *  Created on: 1 Mar 2019
 */

#include "Menu.h"

/*
 * @param LiquidCrystal *lcd - Pointer to the display controller
 * @param DigitalIoPin *b1 - Pointer to the fist menu button - MODE
 * @param DigitalIoPin *b2 - Pointer to the secound menu button - UP
 * @param DigitalIoPin *b3 - Pointer to the third menu button - DOWN
 * @param void (*fPtr)(int) - Pointer to the Sleep function of the main
 */
Menu::Menu(LiquidCrystal *lcd, DigitalIoPin *b1, DigitalIoPin *b2, DigitalIoPin *b3, void (*fPtr)(int)) {
	this->lcd = lcd;
	this->b1 = b1;
	this->b2 = b2;
	this->b3 = b3;
	this->Sleep = fPtr;
	//manualMode = false;
}

Menu::~Menu() {
	clear();
}

/**
 * Clears the display
 */
void Menu::clear(){
	lcd->clear();
}


/**
 * @return the speed value
 * Used in manualmode
 */
uint8_t Menu::getSpeed(){
	return speed;
}

/**
 * Set the speed value to print it on the display
 * Used in automaticmode to display the calculated value
 * @param uint8_t s - calculated speed mode from the automaticmode
 */
void Menu::setSpeed(uint8_t speed){
	if(!manualMode && (speed <= 100 || speed >= 0)){

		if(this->speed != speed){
			changed = true;
			goal = true;
			}
		this->speed = speed;
	}
}

/**
 * @return the displayed pressure value
 *
 */
uint8_t Menu::getPsa(){
	return psa;
}

/**
 * Set the PSA value to print it on the display
 * @param uint8_t psa - calculated pressure value from the sensor
 */
void Menu::setPsa(uint8_t psa){
	if(psa > 0){
		if(this->psa != psa){
			changed = true;
		}
		this->psa = psa;
	}

}

/**
 * Check all buttons if there is some new userinput
 * b1 = UP
 * b2 = Change mode
 * b3 = DOWN
 */
void Menu::checkInputs(){

	if(b1->read()){
		Sleep(3);
		// Longpress
		if(lpb1 % 800){
			if(manualMode){
				if(speed<100){
					speed++;
					changed = true;
				}
			}else{
				if(psa<120){
					psa++;
					changed = true;
				}
			}
		}
		lpb1++;
		if(!b1->read()){// if Manual increase speed
			lpb1 = 0;
			if(manualMode){
				if(speed<100){
					speed++;
					changed = true;
				}
			}else{
				if(psa<120){
					psa++;
					changed = true;
				}
			}

		}
	}
	if(b3->read()){
		Sleep(3);
		// Longpress
		if(lpb3 % 800){
			if(manualMode){
				if(speed>0){
					speed--;
					changed = true;
				}
			}else{
				if(psa>0){
					psa--;
					changed = true;
				}
			}
		}
		lpb3++;
		if(!b3->read()){
			if(manualMode){
				if(speed>0){
					speed--;
					changed = true;
				}
			}else{
				if(psa>0){
					psa--;
					changed = true;
				}
			}
			lpb3=0;
		}
	}

	if(b2->read()){ // Change Mode
		changeMode();
		// blocks all inputs while the button is pressed
		while(b2->read()){};
	}

}

/**
 * Update the data on the display
 */
void Menu::updateDisplay(){
	// Display Speed
	lcd->setCursor(0, 0);
	lcd->print("Fan Speed: ");
	lcd->setCursor(11, 0);
	lcd->print(std::to_string(speed));

	// Display Mode
	lcd->setCursor(15, 0);
	lcd->print(manualMode ? "M" : "A");

	// Display Pressure
	lcd->setCursor(0, 1);
	lcd->print("Pressure: ");
	lcd->setCursor(11, 1);
	lcd->print(std::to_string(psa));
}

/**
 * Display error message for two seconds
 * @param std::string msg - error message (max length 16 characters)
 */
void Menu::error(std::string msg){
	lcd->clear();
	lcd->setCursor(0, 0);
	lcd->print("Error: ");
	lcd->setCursor(0, 1);
	lcd->print(msg);
	Sleep(2000);
	changed= true;
}

/**
 *
 * @return TRUE if mode, speed or pressure changed
 */
bool Menu::hasNewValue(){
	bool val = changed;
	changed = false;
	return val;
}

/**
 * @return TRUE if the pressure changed in automaticmode
 * or the mode is changed to automaticmode
 */
bool Menu::hasNewGoal(){
	bool val = goal;
	goal = false;
	return val;
}
/**
 *  @return the current mode
 *  TRUE = Manualmode
 *  FALSE = Automaticmode
 */
bool Menu::getMode(){
	return manualMode;
}

/**
 * Changed the mode between Manual and Automatic
 */
void Menu::changeMode(){
	manualMode = !manualMode;
	changed = true;
	if(!manualMode) goal=true;
}
