#include "WioExtRTC.h"
#include <climits>

#define PCF8523_CONTROL_1			(0x00)
#define PCF8523_CONTROL_2			(0x01)
#define PCF8523_CONTROL_3			(0x02)
#define PCF8523_SECONDS				(0x03)
#define PCF8523_MINUTES				(0x04)
#define PCF8523_HOURS				(0x05)
#define PCF8523_DAYS				(0x06)
#define PCF8523_WEEKDAYS			(0x07)
#define PCF8523_MONTHS				(0x08)
#define PCF8523_YEARS				(0x09)
#define PCF8523_MINUTE_ALARM		(0x0a)
#define PCF8523_HOUR_ALARM			(0x0b)
#define PCF8523_DAY_ALARM			(0x0c)
#define PCF8523_WEEKDAY_ALARM		(0x0d)
#define PCF8523_OFFSET				(0x0e)
#define PCF8523_TMR_CLOCKOUT_CTRL	(0x0f)
#define PCF8523_TMR_A_FREQ_CTRL		(0x10)
#define PCF8523_TMR_A_REG			(0x11)
#define PCF8523_TMR_B_FREQ_CTRL		(0x12)
#define PCF8523_TMR_B_REG			(0x13)

bool WioExtRTC::Init()
{
	return HwClockOut(true);
}

////////////////////////////////////////////////////////////////////////////////
// RTC functions

bool WioExtRTC::HwClockOut(bool enable)
{
	uint8_t cof = enable ? 0b000 : 0b111;	// CLKOUT = 32768Hz : high-Z
	if (!_Device->ChangeReg8(PCF8523_TMR_CLOCKOUT_CTRL, 0b11000111, cof << 3)) return false;

	return true;
}

bool WioExtRTC::HwTimerBEnable(bool enable)
{
	uint8_t tbc = enable ? 0b1 : 0b0;
	if (!_Device->ChangeReg8(PCF8523_TMR_CLOCKOUT_CTRL, 0b11111110, tbc)) return false;

	return true;
}

bool WioExtRTC::HwTimerBSetPeriod(int sec, int unitSec)
{
	uint8_t tbq;
	switch (unitSec)
	{
	case 1:
		tbq = 0b010;	// 1Hz
		break;
	case 60:
		tbq = 0b011;	// 1/60Hz
		break;
	case 3600:
		tbq = 0b100;	// 1/3600Hz
		break;
	default:
		return false;
	}

	if (!_Device->ChangeReg8(PCF8523_TMR_B_FREQ_CTRL, 0b11111000, tbq)) return false;
	_Device->WriteReg8(PCF8523_TMR_B_REG, sec / unitSec);

	return true;
}

bool WioExtRTC::HwTimerBIsFlag()
{
	uint8_t data;
	if (_Device->ReadReg8(PCF8523_CONTROL_2, &data) != 1) return false;

	return (data & 0b00100000) ? true : false;
}

bool WioExtRTC::HwTimerBClearFlag()
{
	if (!_Device->ChangeReg8(PCF8523_CONTROL_2, 0b11011111, 0b00000000)) return false;

	return true;
}

bool WioExtRTC::HwTimerBEnableInterrupt(bool enable)
{
	uint8_t ctbie = enable ? 0b1 : 0b0;
	if (!_Device->ChangeReg8(PCF8523_CONTROL_2, 0b11111110, ctbie)) return false;

	return true;
}

bool WioExtRTC::SetWakeupPeriod(int sec)
{
	if (sec == INT_MAX)
	{
		if (!HwTimerBEnable(false)) return false;
		if (!HwTimerBClearFlag()) return false;
		if (!HwTimerBEnableInterrupt(false)) return false;

		return true;
	}

	if (sec <= 0 || 255 < sec / 3600) return false;

	if (!HwTimerBEnable(false)) return false;	// --- Disable ---

	if (sec <= 255)
	{
		if (!HwTimerBSetPeriod(sec, 1)) return false;
	}
	else if (sec / 60 <= 255)
	{
		if (!HwTimerBSetPeriod(sec, 60)) return false;
	}
	else
	{
		if (!HwTimerBSetPeriod(sec, 3600)) return false;
	}

	if (!HwTimerBClearFlag()) return false;
	if (!HwTimerBEnableInterrupt(true)) return false;

	if (!HwTimerBEnable(true)) return false;	// --- Enable ---

	return true;
}

bool WioExtRTC::Shutdown()
{
	return HwClockOut(false);
}

////////////////////////////////////////////////////////////////////////////////
// EEPROM functions

void WioExtRTC::EEPROM::Write(uint16_t address, const void* data, int dataSize)
{
	uint8_t writeBuffer[2 + dataSize];
	writeBuffer[0] = address >> 8;
	writeBuffer[1] = address;
	memcpy(&writeBuffer[2], data, dataSize);
	_Device->Write(writeBuffer, 2 + dataSize);
}

bool WioExtRTC::EEPROM::Read(uint16_t address, void* data, int dataSize)
{
	uint8_t writeBuffer[2];
	writeBuffer[0] = address >> 8;
	writeBuffer[1] = address;
	_Device->Write(writeBuffer, 2);

	auto readSize = _Device->Read((uint8_t*)data, dataSize);

	return readSize == dataSize;
}

////////////////////////////////////////////////////////////////////////////////
