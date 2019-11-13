//GROVE_NAME        "Grove - 3-Axis Digital Accelerometer(�}16g)"
//SKU               101020054
//WIKI_URL          http://wiki.seeedstudio.com/Grove-3-Axis_Digital_Accelerometer-16g/

#pragma once

#include "Abstract/GroveModule2.h"
#include "../Connector/GroveConnectorI2C.h"

class GroveAccelerometer16G : public GroveModule2
{
private:
	HalI2CDevice* _Device;

public:
	float X;
	float Y;
	float Z;

public:
	GroveAccelerometer16G(GroveConnectorI2C* connector)
	{
		_Device = connector->NewDevice(0x53);	// I2C_ADDRESS
	}

	bool Init();
	void Read();

};
