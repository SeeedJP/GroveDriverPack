#include "GroveEMGDetector.h"

void GroveEMGDetector::Init()
{
	_Pin->Enable();
}

void GroveEMGDetector::Read()
{
	Level = _Pin->Read();
}
