//GROVE_NAME        "Grove - Laser PM2.5 Air Quality Sensor for Arduino - HM3301"
//SKU               101020613
//WIKI_URL          http://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/

#pragma once

#include "Abstract/GroveModule2.h"
#include "../Connector/GroveConnectorI2C.h"

class GrovePM25HM3301 : public GroveModule2
{
private:
	HalI2CDevice* _Device;

public:
	int Concentration_1_Standard;		// [ug/m3]
	int Concentration_2_5_Standard;		// [ug/m3]
	int Concentration_10_Standard;		// [ug/m3]
	int Concentration_1_Atmospheric;	// [ug/m3]
	int Concentration_2_5_Atmospheric;	// [ug/m3]
	int Concentration_10_Atmospheric;	// [ug/m3]
	int ParticleNumber_0_3;				// [#/l]
	int ParticleNumber_0_5;				// [#/l]
	int ParticleNumber_1;				// [#/l]
	int ParticleNumber_2_5;				// [#/l]
	int ParticleNumber_5;				// [#/l]
	int ParticleNumber_10;				// [#/l]

public:
	GrovePM25HM3301(GroveConnectorI2C* connector)
	{
		_Device = connector->NewDevice(0x40);	// I2C_ADDRESS
	}

	bool Init();
	void Read();

};
