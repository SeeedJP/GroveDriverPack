#include "GroveAirQuality.h"

void GroveAirQuality::Init()
{
	_Pin->Enable();
}

void GroveAirQuality::Read()
{
	float vl = _Pin->Read() * 3.3f;
	float rs = (3.3f - vl) / (vl / 10000.0f);

	Resistance = rs;
}
