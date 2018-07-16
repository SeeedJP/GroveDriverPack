#include "GroveAccelerometer16G.h"
#include <math.h>

#define I2C_ADDRESS   (0x53)
#define REG_POWER_CTL (0x2d)
#define REG_DATAX0    (0x32)

GroveAccelerometer16G::GroveAccelerometer16G(GroveConnectorI2C* connector)
{
	_Device = connector->NewGroveI2CDevice(I2C_ADDRESS);
}

void GroveAccelerometer16G::Initialize()
{
	uint8_t writeData[2];
	writeData[0] = REG_POWER_CTL;
	writeData[1] = 0x08;
	_Device->Write(writeData, sizeof(writeData));
}

void GroveAccelerometer16G::ReadXYZ(float* x, float* y, float* z)
{
	uint8_t writeData[1];
	writeData[0] = REG_DATAX0;
	_Device->Write(writeData, sizeof(writeData));

	uint8_t readData[6];
	if (_Device->Read(readData, sizeof(readData)) != 6)
	{
		throw "exception";
	}

	int16_t val;

	((uint8_t*)&val)[0] = readData[0];
	((uint8_t*)&val)[1] = readData[1];
	*x = val * 2.0 / 512;

	((uint8_t*)&val)[0] = readData[2];
	((uint8_t*)&val)[1] = readData[3];
	*y = val * 2.0 / 512;

	((uint8_t*)&val)[0] = readData[4];
	((uint8_t*)&val)[1] = readData[5];
	*z = val * 2.0 / 512;
}