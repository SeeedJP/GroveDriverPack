//GROVE_NAME        "Grove - Adjustable PIR Motion Sensor"
//SKU               101020617
//WIKI_URL          https://wiki.seeedstudio.com/Grove-Adjustable_PIR_Motion_Sensor/

#pragma once

#include "Abstract/GroveModule2.h"
#include "../Connector/GroveConnectorI2C.h"

#include "Abstract/GroveDigitalInOnOff.h"

class GrovePIR : public GroveDigitalInOnOff
{
public:
	GrovePIR(GroveConnectorDIO* connector) : GroveDigitalInOnOff(connector)
	{
	}

	GrovePIR(HalGpio* pin) : GroveDigitalInOnOff(pin)
	{
	}

};
