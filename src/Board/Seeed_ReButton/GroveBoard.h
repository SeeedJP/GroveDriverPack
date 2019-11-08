#pragma once

#if defined ARDUINO_Seeed_ReButton

#include <mbed.h>

class GroveBoard
{
private:
	mbed::I2C _I2c;

public:
	GroveConnectorI2C I2C;

public:
	GroveBoard() :
		_I2c(PB_9, PB_8),
		I2C(&_I2c)
	{
	}

};

#endif // ARDUINO_Seeed_ReButton
