#pragma once

#include "GroveConnector.h"
#include "../HAL/Hal.h"

class GroveConnectorDIO : public GroveConnector
{
public:
	HalGpio P1;
	HalGpio P2;

public:
	GroveConnectorDIO(HalPlatformPinType pin1, HalPlatformPinType pin2) : P1(pin1), P2(pin2)
	{
	}

	void Enable()
	{
	}

};
