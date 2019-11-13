#include "GroveTempHumiSHT3x.h"
#include "../../HAL/Hal.h"

#define POLYNOMIAL			(0x31)

#define CMD_SOFT_RESET		(0x30a2)
#define CMD_SINGLE_HIGH		(0x2400)
#define CMD_SINGLE_MEDIUM	(0x240b)
#define CMD_SINGLE_LOW		(0x2416)
#define CMD_HEATER_ON		(0x306d)
#define CMD_HEATER_OFF		(0x3066)

void GroveTempHumiSHT3x::SendCommand(uint16_t cmd)
{
	uint8_t writeData[2];
	writeData[0] = cmd >> 8;
	writeData[1] = cmd & 0xff;
	_Device->Write(writeData, sizeof(writeData));
}

uint8_t GroveTempHumiSHT3x::CalcCRC8(const uint8_t* data, int dataSize)
{
	uint8_t crc = 0xff;

	for (int j = dataSize; j > 0; j--)
	{
		crc ^= *data++;

		for (int i = 8; i > 0; i--)
		{
			crc = crc & 0x80 ? crc << 1 ^ POLYNOMIAL : crc << 1;
		}
	}

	return crc;
}

void GroveTempHumiSHT3x::SetRepeatability(REPEATABILITY repeatability)
{
	_Repeatability = repeatability;
}

bool GroveTempHumiSHT3x::Init()
{
	if (!_Device->IsExist()) return false;

	SendCommand(CMD_SOFT_RESET);
	HalSystem::DelayMs(1);

	_IsExist = true;
	return true;
}

void GroveTempHumiSHT3x::Read()
{
	if (!_IsExist)
	{
		Temperature = NAN;
		Humidity = NAN;
		return;
	}

	uint16_t cmd;
	int duration;
	switch (_Repeatability)
	{
	case REPEATABILITY_HIGH:
		cmd = CMD_SINGLE_HIGH;
		duration = 15;
		break;
	case REPEATABILITY_MEDIUM:
		cmd = CMD_SINGLE_MEDIUM;
		duration = 6;
		break;
	case REPEATABILITY_LOW:
		cmd = CMD_SINGLE_LOW;
		duration = 4;
		break;
	default:
		HalSystem::Abort();
	}

	SendCommand(cmd);
	HalSystem::DelayMs(duration);

	uint8_t readData[6];
	if (_Device->Read(readData, sizeof(readData)) != 6) HalSystem::Abort();

	if (readData[2] != CalcCRC8(&readData[0], 2)) HalSystem::Abort();
	if (readData[5] != CalcCRC8(&readData[3], 2)) HalSystem::Abort();

	uint16_t ST;
	ST = readData[0];
	ST <<= 8;
	ST |= readData[1];

	uint16_t SRH;
	SRH = readData[3];
	SRH <<= 8;
	SRH |= readData[4];

	Temperature = (float)ST * 175 / 0xffff - 45;
	Humidity = (float)SRH * 100 / 0xffff;
}

void GroveTempHumiSHT3x::SetHeater(bool on)
{
	if (!_IsExist) return;

	SendCommand(on ? CMD_HEATER_ON : CMD_HEATER_OFF);
}
