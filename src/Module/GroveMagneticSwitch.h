//GROVE_NAME        "Grove - Magnetic Switch"
//SKU               101020038
//WIKI_URL          http://wiki.seeedstudio.com/Grove-Magnetic_Switch/

#pragma once

#include "Abstract/GroveDigitalInOnOff.h"

class GroveMagneticSwitch : public GroveDigitalInOnOff
{
public:
	GroveMagneticSwitch(GroveConnectorDIO* connector) : GroveDigitalInOnOff(connector)
	{
	}

	GroveMagneticSwitch(GroveGpio* pin) : GroveDigitalInOnOff(pin)
	{
	}

};
