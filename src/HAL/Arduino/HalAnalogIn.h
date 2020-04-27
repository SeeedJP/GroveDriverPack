#pragma once

#include "../HalAnalogInBase.h"
#include <Arduino.h>

class HalAnalogIn : public HalAnalogInBase
{
private:
	int _Pin;

public:
	HalAnalogIn(int pin)
	{
		_Pin = pin;
	}

protected:
	virtual void EnableImplement()
	{
#if defined ARDUINO_ARCH_STM32F4 || defined ARDUINO_ARCH_STM32
		pinMode(_Pin, INPUT_ANALOG);
#endif // ARDUINO_ARCH_STM32F4 || ARDUINO_ARCH_STM32
	}

	virtual float ReadImplement()
	{
		return (float)analogRead(_Pin) / 1023;
	}

};
