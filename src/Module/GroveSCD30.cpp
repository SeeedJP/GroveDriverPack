#include "GroveSCD30.h"
#include "../HAL/Hal.h"

uint8_t GroveSCD30::CalcCRC(const void* data, int dataSize) const
{
	uint8_t crc = 0xff;

	for (int i = 0; i < dataSize; ++i)
	{
		crc ^= static_cast<const uint8_t*>(data)[i];

		for (int bit = 8; bit > 0; --bit)
		{
			if (crc & 0x80)
			{
				crc = (crc << 1) ^ 0x31;  // x^8 + x^5 + x^4 + 1
			}
			else
			{
				crc = (crc << 1);
			}
		}
	}

	return crc;
}

void GroveSCD30::Write(uint16_t cmd, const uint16_t* data, int dataNumber)
{
	const int writeDataSize{ 2 + 3 * dataNumber };
	uint8_t writeData[writeDataSize];
	writeData[0] = cmd >> 8;
	writeData[1] = cmd;
	for (int i = 0; i < dataNumber; ++i)
	{
		writeData[2 + i * 3 + 0] = data[i] >> 8;
		writeData[2 + i * 3 + 1] = data[i];
		writeData[2 + i * 3 + 2] = CalcCRC(&writeData[2 + i * 3 + 0], 2);
	}

	_Device->Write(writeData, writeDataSize);
}

int GroveSCD30::Read(uint16_t address, uint16_t* data, int dataNumber)
{
	const int writeDataSize{ 2 };
	uint8_t writeData[writeDataSize];
	writeData[0] = address >> 8;
	writeData[1] = address;

	_Device->Write((uint8_t*)writeData, writeDataSize);

	const int readDataSize{ 3 * dataNumber };
	uint8_t readData[readDataSize];

	HalSystem::DelayMs(3);

	if (_Device->Read(readData, readDataSize) != readDataSize) return 0;

	for (int i = 0; i < dataNumber; ++i)
	{
		if (CalcCRC(&readData[i * 3 + 0], 2) != readData[i * 3 + 2]) return i;
		data[i] = readData[i * 3 + 0] << 8 | readData[i * 3 + 1];
	}

	return dataNumber;
}

void GroveSCD30::TriggerContinuousMeasurement(uint16_t pressure)
{
	Write(0x0010, &pressure, 1);
}

void GroveSCD30::StopContinuousMeasurement()
{
	Write(0x0104, nullptr, 0);
}

void GroveSCD30::SetMeasurementInterval(uint16_t interval)
{
	Write(0x4600, &interval, 1);
}

int GroveSCD30::GetMeasurementInterval()
{
	uint16_t interval;

	if (Read(0x4600, &interval, 1) != 1) return -1;

	return interval;
}

int GroveSCD30::GetDataReadyStatus()
{
	uint16_t dataReady;

	if (Read(0x0202, &dataReady, 1) != 1) return -1;

	return dataReady;
}

bool GroveSCD30::ReadMeasurement(float* co2, float* t, float* rh)
{
	uint16_t data[6];

	if (Read(0x0300, data, sizeof(data) / sizeof(data[0])) != sizeof(data) / sizeof(data[0])) return false;
	uint32_t tmp;
	tmp = data[0] << 16 | data[1];
	*co2 = *(float*)&tmp;
	tmp = data[2] << 16 | data[3];
	*t = *(float*)&tmp;
	tmp = data[4] << 16 | data[5];
	*rh = *(float*)&tmp;

	return true;
}

bool GroveSCD30::Init()
{
	if (!_Device->IsExist()) return false;

	SetMeasurementInterval(2);
	TriggerContinuousMeasurement();

	_IsExist = true;
	return true;
}

bool GroveSCD30::ReadyToRead()
{
	if (!_IsExist) return false;

	return GetDataReadyStatus() == 1 ? true : false;
}


void GroveSCD30::Read()
{
	if (!_IsExist)
	{
		Temperature = NAN;
		Humidity = NAN;
		Co2Concentration = NAN;
		return;
	}

	if (!ReadyToRead()) return;

	float co2;
	float t;
	float rh;
	if (!ReadMeasurement(&co2, &t, &rh))
	{
		Temperature = NAN;
		Humidity = NAN;
		Co2Concentration = NAN;
		return;
	}

	Temperature = t;
	Humidity = rh;
	Co2Concentration = co2;
}
