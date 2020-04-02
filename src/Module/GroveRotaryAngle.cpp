#include "GroveRotaryAngle.h"

void GroveRotaryAngle::Init()
{
	_Pin->Enable();
}

void GroveRotaryAngle::Read()
{
	Volume = _Pin->Read();
}
