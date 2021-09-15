#pragma once

#include <Arduino.h>
#include "../HalUARTBase.h"

class HalUART : public HalUARTBase
{
private:
	HardwareSerial* _Serial;

public:
	HalUART(HardwareSerial* serial) : _Serial(serial)
	{
	}

protected:
	virtual void EnableImplement(int baudRate, int dataBit, ParityType parity, int stopBit);
	virtual void WriteImplement(uint8_t data);
	virtual int ReadAvailableImplement();
	virtual uint8_t ReadImplement();

};
