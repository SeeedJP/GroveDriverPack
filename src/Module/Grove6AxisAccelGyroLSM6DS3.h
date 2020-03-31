//GROVE_NAME        "Grove - 6-Axis Accelerometer&Gyroscope"
//SKU               105020012
//WIKI_URL          http://wiki.seeedstudio.com/Grove-6-Axis_AccelerometerAndGyroscope/

#pragma once

#include "Abstract/GroveModule2.h"
#include "../Connector/GroveConnectorI2C.h"

class Grove6AxisAccelGyroLSM6DS3 : public GroveModule2
{
public:
    enum lsm6ds3_mode_t
    {
        MODE_BASIC_MEASURE,
        MODE_STEP_COUNTER,
        MODE_FREEFALL_DETECTION,
    };

private:
	HalI2CDevice* _Device;
    lsm6ds3_mode_t _mode;

public:
	float accelX;
	float accelY;
	float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;

public:
	Grove6AxisAccelGyroLSM6DS3(GroveConnectorI2C* connector)
	{
		_Device = connector->NewDevice(0x6A);	// I2C_ADDRESS
        _mode = MODE_BASIC_MEASURE;
	}

	bool Init();
    void Setup(lsm6ds3_mode_t mode);

	void ReadAccel();
	void ReadGyro();

    uint16_t GetStepCounter();
    void ClearStepCounter();

    bool WaitForFreeFallEvent(uint32_t timeout_ms, uint32_t polling_ms);
};
