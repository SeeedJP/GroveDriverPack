#include "GrovePM25HM3301.h"
#include <climits>

bool GrovePM25HM3301::Init()
{
	if (!_Device->IsExist()) return false;

	_IsExist = true;
	return true;
}

void GrovePM25HM3301::Read()
{
	const uint8_t writeData[]{ 0x88, };
	_Device->Write(writeData, sizeof(writeData));

	uint8_t readData[29];
	auto readSize = _Device->Read(readData, sizeof(readData));

	uint8_t sum = 0;
	for (int i = 0; i < sizeof(readData) - 1; ++i) sum += readData[i];

	if (readSize != sizeof(readData) || sum != readData[28])
	{
		Concentration_1_Standard      = INT_MAX;
		Concentration_2_5_Standard    = INT_MAX;
		Concentration_10_Standard     = INT_MAX;
		Concentration_1_Atmospheric   = INT_MAX;
		Concentration_2_5_Atmospheric = INT_MAX;
		Concentration_10_Atmospheric  = INT_MAX;
		ParticleNumber_0_3            = INT_MAX;
		ParticleNumber_0_5            = INT_MAX;
		ParticleNumber_1              = INT_MAX;
		ParticleNumber_2_5            = INT_MAX;
		ParticleNumber_5              = INT_MAX;
		ParticleNumber_10             = INT_MAX;
		return;
	}

	Concentration_1_Standard      = readData[4]  * 256 + readData[5] ;
	Concentration_2_5_Standard    = readData[6]  * 256 + readData[7] ;
	Concentration_10_Standard     = readData[8]  * 256 + readData[9] ;
	Concentration_1_Atmospheric   = readData[10] * 256 + readData[11];
	Concentration_2_5_Atmospheric = readData[12] * 256 + readData[13];
	Concentration_10_Atmospheric  = readData[14] * 256 + readData[15];
	ParticleNumber_0_3            = readData[16] * 256 + readData[17];
	ParticleNumber_0_5            = readData[18] * 256 + readData[19];
	ParticleNumber_1              = readData[20] * 256 + readData[21];
	ParticleNumber_2_5            = readData[22] * 256 + readData[23];
	ParticleNumber_5              = readData[24] * 256 + readData[25];
	ParticleNumber_10             = readData[26] * 256 + readData[27];
}
