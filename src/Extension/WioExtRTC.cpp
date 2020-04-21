#include "WioExtRTC.h"
#include <cassert>
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

static int Weekday(int year, int month, int day)
{
	return (day += month < 3 ? year-- : year - 2, 23 * month / 9 + day + 4 + year / 4 - year / 100 + year / 400) % 7;
}

static uint8_t EncodeBCD(uint8_t value)
{
	assert(value <= 99);
	return value / 10 * 0x10 + value % 10;
}

static uint8_t DecodeBCD(uint8_t value)
{
	return value / 0x10 * 10 + value % 0x10;
}

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

bool WioExtRTC::HwSetDateTime(int year2digit, int month, int day, int weekday, int hour, int minute, int second)
{
	if (!(0 <= year2digit && year2digit <= 99)) return false;
	if (!(1 <= month && month <= 12)) return false;
	if (!(1 <= day && day <= 31)) return false;
	if (!(0 <= weekday && weekday <= 6)) return false;
	if (!(0 <= hour && hour <= 23)) return false;
	if (!(0 <= minute && minute <= 59)) return false;
	if (!(0 <= second && second <= 59)) return false;

	uint8_t data[7];
	data[0] = EncodeBCD(second);
	data[1] = EncodeBCD(minute);
	data[2] = EncodeBCD(hour);
	data[3] = EncodeBCD(day);
	data[4] = EncodeBCD(weekday);
	data[5] = EncodeBCD(month);
	data[6] = EncodeBCD(year2digit);

	_Device->WriteRegN(PCF8523_SECONDS, data, sizeof(data));

	_Device->WriteReg8(PCF8523_TMR_A_FREQ_CTRL, 0b00000110);	// Change to other than initial value. TAQ = 0b110

	return true;
}

bool WioExtRTC::HwGetDateTime(int& year2digit, int& month, int& day, int& weekday, int& hour, int& minute, int& second)
{
	uint8_t freqCtrl;
	if (_Device->ReadReg8(PCF8523_TMR_A_FREQ_CTRL, &freqCtrl) != 1) return false;
	if ((freqCtrl & 0b00000111) == 0b00000111)	// Is it initial value? TAQ = 0b111
	{
		year2digit = 0;
		month = 0;
		day = 0;
		weekday = 0;
		hour = 0;
		minute = 0;
		second = 0;

		return true;
	}

	uint8_t data[7];
	if (_Device->ReadRegN(PCF8523_SECONDS, data, sizeof(data)) != sizeof(data)) return false;

	second = DecodeBCD(data[0] & 0x7f);
	minute = DecodeBCD(data[1] & 0x7f);
	hour = DecodeBCD(data[2] & 0x3f);
	day = DecodeBCD(data[3] & 0x3f);
	weekday = DecodeBCD(data[4] & 0x07);
	month = DecodeBCD(data[5] & 0x1f);
	year2digit = DecodeBCD(data[6]);

	return true;
}

bool WioExtRTC::SetDateTime(int year, int month, int day, int hour, int minute, int second)
{
	if (!(1980 <= year && year <= 2079)) return false;

	return HwSetDateTime(year % 100, month, day, Weekday(year, month, day), hour, minute, second);
}

bool WioExtRTC::SetDateTime(const tm& tim)
{
	return SetDateTime(tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);
}

bool WioExtRTC::GetDateTime(int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	int year2digit;
	int weekday;
	if (!HwGetDateTime(year2digit, month, day, weekday, hour, minute, second)) return false;

	if (year2digit == 0)
	{
		year = 0;
	}
	else
	{
		year = (year2digit >= 80 ? 1900 : 2000) + year2digit;
	}

	return true;
}

bool WioExtRTC::GetDateTime(tm& tim)
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	if (!GetDateTime(year, month, day, hour, minute, second)) return false;
	if (year == 0)
	{
		year = 1970;
		month = 1;
		day = 1;
		hour = 0;
		minute = 0;
		second = 0;
	}

	memset(&tim, 0, sizeof(tim));
	tim.tm_year = year - 1900;
	tim.tm_mon = month - 1;
	tim.tm_mday = day;
	tim.tm_hour = hour;
	tim.tm_min = minute;
	tim.tm_sec = second;

	// Update tm_wday and tm_yday
	mktime(&tim);

	return true;
}

bool WioExtRTC::IsWakeupReasonPeriod()
{
	return HwTimerBIsFlag();
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
