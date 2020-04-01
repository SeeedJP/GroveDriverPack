//GROVE_NAME        "Grove - 6-Axis Accelerometer&Gyroscope"
//SKU               105020012
//WIKI_URL          http://wiki.seeedstudio.com/Grove-6-Axis_AccelerometerAndGyroscope/

#pragma once

#include "Abstract/GroveModule2.h"
#include "../Connector/GroveConnectorI2C.h"

class Grove6AxisAccelGyroLSM6DS3 : public GroveModule2
{
private:
	HalI2CDevice* _Device;

public:
	float AccelX;
	float AccelY;
	float AccelZ;
    float GyroX;
    float GyroY;
    float GyroZ;

public:
	Grove6AxisAccelGyroLSM6DS3(GroveConnectorI2C* connector)
	{
		_Device = connector->NewDevice(0x6A);	// I2C_ADDRESS
	}

	bool Init();
	void Read();
};
