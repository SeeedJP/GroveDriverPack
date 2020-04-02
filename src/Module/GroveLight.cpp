#include "GroveLight.h"

void GroveLight::Init()
{
	_Pin->Enable();
}

void GroveLight::Read()
{
	Value = _Pin->Read();
}
