//GROVE_NAME        "Grove - CO2 & Temperature & Humidity Sensor for Arduino (SCD30) - 3-in-1"
//SKU               101020634
//WIKI_URL          https://wiki.seeedstudio.com/Grove-CO2_Temperature_Humidity_Sensor-SCD30/

#pragma once

#include "Abstract/GroveModule2.h"
#include "../Connector/GroveConnectorI2C.h"

class GroveSCD30 : public GroveModule2
{
private:
	HalI2CDevice * _Device;

	uint8_t CalcCRC(const void* data, int dataSize) const;

	void Write(uint16_t cmd, const uint16_t* data, int dataNumber);
	int Read(uint16_t address, uint16_t* data, int dataNumber);

	void TriggerContinuousMeasurement(uint16_t pressure = 0);
	void StopContinuousMeasurement();
	void SetMeasurementInterval(uint16_t interval);
	int GetMeasurementInterval();
	int GetDataReadyStatus();
	bool ReadMeasurement(float* co2, float* t, float* rh);

public:
	float Temperature;
	float Humidity;
	float Co2Concentration;

public:
	GroveSCD30(GroveConnectorI2C* connector)
	{
		_Device = connector->NewDevice(0x61);	// I2C_ADDRESS
	}

	bool Init();
	bool ReadyToRead();
	void Read();

};
