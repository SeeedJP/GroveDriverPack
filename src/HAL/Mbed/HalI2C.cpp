#if defined __MBED__

#include "HalI2C.h"

void HalI2C::EnableImplement()
{
}

bool HalI2C::WriteImplement(int slaveAddress, const uint8_t* data, int dataSize)
{
	if (_I2C->write(slaveAddress << 1, (const char*)data, dataSize) != 0) return false;
	return true;
}

int HalI2C::ReadImplement(int slaveAddress, uint8_t* data, int dataSize)
{
	if (_I2C->read(slaveAddress << 1, (char*)data, dataSize) != 0) return 0;
	return dataSize;
}

#endif // __MBED__
