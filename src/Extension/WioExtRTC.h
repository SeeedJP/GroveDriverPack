//PRODUCT_NAME      "Wio Extension - RTC"
//SKU               103100082
//WIKI_URL          https://www.seeedstudio.com/Wio-Extension-RTC-p-4002.html

#pragma once

#include "../Connector/GroveConnectorI2C.h"

class WioExtRTC
{
public:
	class EEPROM
	{
	private:
		HalI2CDevice* _Device;

	public:
		EEPROM(GroveConnectorI2C* connector)
		{
			_Device = connector->NewDevice(0xa0 >> 1);	// 24C32
		}

		void Write(uint16_t address, const void* data, int dataSize);
		bool Read(uint16_t address, void* data, int dataSize);

	};

private:
	HalI2CDevice* _Device;

	bool HwClockOut(bool enable);
	bool HwTimerBEnable(bool enable);
	bool HwTimerBSetPeriod(int sec, int unitSec);
	bool HwTimerBIsFlag();
	bool HwTimerBClearFlag();
	bool HwTimerBEnableInterrupt(bool enable);
	bool HwSetDateTime(int year2digit, int month, int day, int weekday, int hour, int minute, int second);
	bool HwGetDateTime(int& year2digit, int& month, int& day, int& weekday, int& hour, int& minute, int& second);

public:
	EEPROM Eeprom;

public:
	WioExtRTC(GroveConnectorI2C* connector) : Eeprom(connector)
	{
		_Device = connector->NewDevice(0xd0 >> 1);		// PCF8523
	}

	bool Init();
	bool SetDateTime(int year, int month, int day, int hour, int minute, int second);
	bool GetDateTime(int& year, int& month, int& day, int& hour, int& minute, int& second);
	bool IsWakeupReasonPeriod();
	bool SetWakeupPeriod(int sec);
	bool Shutdown();

};
