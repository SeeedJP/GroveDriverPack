#pragma once

#if defined ARDUINO_WIO_TERMINAL

class GroveBoard
{
public:
	GroveConnectorDIO GroveBCM27;
	GroveConnectorDIO& GroveD0;
	GroveConnectorAnalogIn GroveA0;

	GroveConnectorDIO GroveBCM3;
	GroveConnectorI2C GroveI2C1;

public:
	GroveBoard() :
		GroveBCM27{ BCM27, BCM22 },
		GroveD0{ GroveBCM27 },
		GroveA0{ A0, A1 },
		GroveBCM3{ BCM3, BCM2 },
		GroveI2C1{ &Wire }
	{
	}

};

#endif // ARDUINO_WIO_TERMINAL
