#pragma once

#include "GroveConnector.h"
#include "../HAL/Hal.h"

class GroveConnectorI2C : public GroveConnector
{
private:
	HalI2C _I2C;

public:
	GroveConnectorI2C(HalPlatformI2CType* i2c) : _I2C(i2c)
	{
	}

	void Enable()
	{
		_I2C.Enable();
	}

	HalI2CDevice* NewDevice(uint8_t slaveAddress)
	{
		return new HalI2CDevice(&_I2C, slaveAddress);
	}

};
