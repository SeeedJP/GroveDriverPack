#pragma once

#include "GroveModule.h"
#include "../../Connector/GroveConnectorI2C.h"

class GroveTempHumiSHT3x : public GroveModule
{
public:
	enum REPEATABILITY
	{
		REPEATABILITY_HIGH,
		REPEATABILITY_MEDIUM,
		REPEATABILITY_LOW,
	};

protected:
	HalI2CDevice* _Device;
	REPEATABILITY _Repeatability;

	void SendCommand(uint16_t cmd);
	static uint8_t CalcCRC8(const uint8_t* data, int dataSize);

public:
	float Temperature;
	float Humidity;

protected:
	GroveTempHumiSHT3x(GroveConnectorI2C* connector, uint8_t slaveAddress)
	{
		_Repeatability = REPEATABILITY_HIGH;

		_Device = connector->NewDevice(slaveAddress);	// I2C_ADDRESS
	}

public:
	void SetRepeatability(REPEATABILITY repeatability);

	void Init();
	void Read();
	void SetHeater(bool on);

};
