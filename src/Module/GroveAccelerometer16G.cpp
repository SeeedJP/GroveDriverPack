#include "GroveAccelerometer16G.h"
#include <math.h>

#define REG_POWER_CTL (0x2d)
#define REG_DATAX0    (0x32)

bool GroveAccelerometer16G::Init()
{
	if (!_Device->IsExist()) return false;

	_Device->WriteReg8(REG_POWER_CTL, 0x08);

	_IsExist = true;
	return true;
}

void GroveAccelerometer16G::Read()
{
	if (!_IsExist)
	{
		X = NAN;
		Y = NAN;
		Z = NAN;
		return;
	}

	uint8_t readData[6];
	if (_Device->ReadRegN(REG_DATAX0, readData, sizeof(readData)) != 6) HalSystem::Abort();

	int16_t val;

	((uint8_t*)&val)[0] = readData[0];
	((uint8_t*)&val)[1] = readData[1];
	X = val * 2.0 / 512;

	((uint8_t*)&val)[0] = readData[2];
	((uint8_t*)&val)[1] = readData[3];
	Y = val * 2.0 / 512;

	((uint8_t*)&val)[0] = readData[4];
	((uint8_t*)&val)[1] = readData[5];
	Z = val * 2.0 / 512;
}
