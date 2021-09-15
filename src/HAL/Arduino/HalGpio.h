#pragma once

#include <Arduino.h>
#include "../HalGpioBase.h"

class HalGpio : public HalGpioBase
{
private:
	int _Pin;

public:
	HalGpio(int pin)
	{
		_Pin = pin;
	}

protected:
	virtual void EnableImplement(ModeType mode)
	{
		switch (mode)
		{
		case MODE_INPUT:
			pinMode(_Pin, INPUT);
			break;
		case MODE_OUTPUT:
			pinMode(_Pin, OUTPUT);
			break;
#if defined ARDUINO_ARCH_STM32F4 || defined ARDUINO_ARCH_STM32
		case MODE_OUTPUT_OPEN_DRAIN:
			pinMode(_Pin, OUTPUT_OPEN_DRAIN);
			break;
#endif // ARDUINO_ARCH_STM32F4 || ARDUINO_ARCH_STM32
		default:
			abort();
		}
	}

	virtual void WriteImplement(bool high)
	{
		digitalWrite(_Pin, high ? HIGH : LOW);
	}

	virtual bool ReadImplement() const
	{
		return digitalRead(_Pin) == HIGH;
	}

};
