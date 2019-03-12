/*
 * AutoCon.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: vovan
 */

#include "AutoCon.h"

#include "ITM_conv.h"
bool setFrequency(ModbusMaster& node, uint16_t freq, void (*Sleep)(int))
{
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	ModbusRegister Frequency(&node, 1); // reference 1
	ModbusRegister StatusWord(&node, 3);

	Frequency = freq; // set motor frequency

	//p.print("\nSet freq = %d"); // for debugging
	//p.print((int) ctr * delay);


	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = StatusWord;
		// check if we are at setpoint
		if (result >= 0 && (result & 0x0100)) atSetpoint = true;
		ctr++;
	} while(ctr < 20 && !atSetpoint);
	//p.print("\n El:"); // for debugging
	//p.print((int) ctr * delay);
	return atSetpoint;
}

AutoCon::AutoCon(I2CMaster* i2c)//UNFINISHED
{
	uint8_t received[3];
	i2c->ReadValueI2CM(received, 3);
	baseVal = received[1];
}

void AutoCon::adjust(I2CMaster* I2Cread, ModbusMaster& mbWrite, Menu* menu, void (*Sleep)(int))//UNFINISHED
{
	uint8_t received[3];
	I2Cread->ReadValueI2CM(received, 3);
	//READING VALUES FOR TESTING
	p.print("\n Values: ");
	p.print(received[0]);

	p.print(" - ");
	p.print(baseVal);

	int ratio = received[0]/baseVal;
	setFrequency(mbWrite, (uint16_t) menu->getSpeed()+(menu->getSpeed()*ratio), Sleep);

}





