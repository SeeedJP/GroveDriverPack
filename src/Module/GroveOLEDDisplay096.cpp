#include "GroveOLEDDisplay096.h"
#include "../HAL/Hal.h"

#define COMMAND_MODE        (0x80)
#define DATA_MODE			(0x40)
#define DISPLAY_OFF_CMD		(0xae)
#define DISPLAY_ON_CMD		(0xaf)
#define NORMAL_DISPLAY_CMD  (0xa6)

static const uint8_t BasicFont[][8] =
{
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },	// 0x20
	{ 0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00 },	// 0x21
	{ 0x00,0x00,0x07,0x00,0x07,0x00,0x00,0x00 },	// 0x22
	{ 0x00,0x14,0x7F,0x14,0x7F,0x14,0x00,0x00 },	// 0x23
	{ 0x00,0x24,0x2A,0x7F,0x2A,0x12,0x00,0x00 },	// 0x24
	{ 0x00,0x23,0x13,0x08,0x64,0x62,0x00,0x00 },	// 0x25
	{ 0x00,0x36,0x49,0x55,0x22,0x50,0x00,0x00 },	// 0x26
	{ 0x00,0x00,0x05,0x03,0x00,0x00,0x00,0x00 },	// 0x27
	{ 0x00,0x1C,0x22,0x41,0x00,0x00,0x00,0x00 },	// 0x28
	{ 0x00,0x41,0x22,0x1C,0x00,0x00,0x00,0x00 },	// 0x29
	{ 0x00,0x08,0x2A,0x1C,0x2A,0x08,0x00,0x00 },	// 0x2a
	{ 0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00 },	// 0x2b
	{ 0x00,0xA0,0x60,0x00,0x00,0x00,0x00,0x00 },	// 0x2c
	{ 0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00 },	// 0x2d
	{ 0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x00 },	// 0x2e
	{ 0x00,0x20,0x10,0x08,0x04,0x02,0x00,0x00 },	// 0x2f
	{ 0x00,0x3E,0x51,0x49,0x45,0x3E,0x00,0x00 },	// 0x30
	{ 0x00,0x00,0x42,0x7F,0x40,0x00,0x00,0x00 },	// 0x31
	{ 0x00,0x62,0x51,0x49,0x49,0x46,0x00,0x00 },	// 0x32
	{ 0x00,0x22,0x41,0x49,0x49,0x36,0x00,0x00 },	// 0x33
	{ 0x00,0x18,0x14,0x12,0x7F,0x10,0x00,0x00 },	// 0x34
	{ 0x00,0x27,0x45,0x45,0x45,0x39,0x00,0x00 },	// 0x35
	{ 0x00,0x3C,0x4A,0x49,0x49,0x30,0x00,0x00 },	// 0x36
	{ 0x00,0x01,0x71,0x09,0x05,0x03,0x00,0x00 },	// 0x37
	{ 0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x00 },	// 0x38
	{ 0x00,0x06,0x49,0x49,0x29,0x1E,0x00,0x00 },	// 0x39
	{ 0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00 },	// 0x3a
	{ 0x00,0x00,0xAC,0x6C,0x00,0x00,0x00,0x00 },	// 0x3b
	{ 0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x00 },	// 0x3c
	{ 0x00,0x14,0x14,0x14,0x14,0x14,0x00,0x00 },	// 0x3d
	{ 0x00,0x41,0x22,0x14,0x08,0x00,0x00,0x00 },	// 0x3e
	{ 0x00,0x02,0x01,0x51,0x09,0x06,0x00,0x00 },	// 0x3f
	{ 0x00,0x32,0x49,0x79,0x41,0x3E,0x00,0x00 },	// 0x40
	{ 0x00,0x7E,0x09,0x09,0x09,0x7E,0x00,0x00 },	// 0x41
	{ 0x00,0x7F,0x49,0x49,0x49,0x36,0x00,0x00 },	// 0x42
	{ 0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x00 },	// 0x43
	{ 0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x00 },	// 0x44
	{ 0x00,0x7F,0x49,0x49,0x49,0x41,0x00,0x00 },	// 0x45
	{ 0x00,0x7F,0x09,0x09,0x09,0x01,0x00,0x00 },	// 0x46
	{ 0x00,0x3E,0x41,0x41,0x51,0x72,0x00,0x00 },	// 0x47
	{ 0x00,0x7F,0x08,0x08,0x08,0x7F,0x00,0x00 },	// 0x48
	{ 0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00 },	// 0x49
	{ 0x00,0x20,0x40,0x41,0x3F,0x01,0x00,0x00 },	// 0x4a
	{ 0x00,0x7F,0x08,0x14,0x22,0x41,0x00,0x00 },	// 0x4b
	{ 0x00,0x7F,0x40,0x40,0x40,0x40,0x00,0x00 },	// 0x4c
	{ 0x00,0x7F,0x02,0x0C,0x02,0x7F,0x00,0x00 },	// 0x4d
	{ 0x00,0x7F,0x04,0x08,0x10,0x7F,0x00,0x00 },	// 0x4e
	{ 0x00,0x3E,0x41,0x41,0x41,0x3E,0x00,0x00 },	// 0x4f
	{ 0x00,0x7F,0x09,0x09,0x09,0x06,0x00,0x00 },	// 0x50
	{ 0x00,0x3E,0x41,0x51,0x21,0x5E,0x00,0x00 },	// 0x51
	{ 0x00,0x7F,0x09,0x19,0x29,0x46,0x00,0x00 },	// 0x52
	{ 0x00,0x26,0x49,0x49,0x49,0x32,0x00,0x00 },	// 0x53
	{ 0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x00 },	// 0x54
	{ 0x00,0x3F,0x40,0x40,0x40,0x3F,0x00,0x00 },	// 0x55
	{ 0x00,0x1F,0x20,0x40,0x20,0x1F,0x00,0x00 },	// 0x56
	{ 0x00,0x3F,0x40,0x38,0x40,0x3F,0x00,0x00 },	// 0x57
	{ 0x00,0x63,0x14,0x08,0x14,0x63,0x00,0x00 },	// 0x58
	{ 0x00,0x03,0x04,0x78,0x04,0x03,0x00,0x00 },	// 0x59
	{ 0x00,0x61,0x51,0x49,0x45,0x43,0x00,0x00 },	// 0x5a
	{ 0x00,0x7F,0x41,0x41,0x00,0x00,0x00,0x00 },	// 0x5b
	{ 0x00,0x02,0x04,0x08,0x10,0x20,0x00,0x00 },	// 0x5c
	{ 0x00,0x41,0x41,0x7F,0x00,0x00,0x00,0x00 },	// 0x5d
	{ 0x00,0x04,0x02,0x01,0x02,0x04,0x00,0x00 },	// 0x5e
	{ 0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00 },	// 0x5f
	{ 0x00,0x01,0x02,0x04,0x00,0x00,0x00,0x00 },	// 0x60
	{ 0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00 },	// 0x61
	{ 0x00,0x7F,0x48,0x44,0x44,0x38,0x00,0x00 },	// 0x62
	{ 0x00,0x38,0x44,0x44,0x28,0x00,0x00,0x00 },	// 0x63
	{ 0x00,0x38,0x44,0x44,0x48,0x7F,0x00,0x00 },	// 0x64
	{ 0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00 },	// 0x65
	{ 0x00,0x08,0x7E,0x09,0x02,0x00,0x00,0x00 },	// 0x66
	{ 0x00,0x18,0xA4,0xA4,0xA4,0x7C,0x00,0x00 },	// 0x67
	{ 0x00,0x7F,0x08,0x04,0x04,0x78,0x00,0x00 },	// 0x68
	{ 0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x00 },	// 0x69
	{ 0x00,0x80,0x84,0x7D,0x00,0x00,0x00,0x00 },	// 0x6a
	{ 0x00,0x7F,0x10,0x28,0x44,0x00,0x00,0x00 },	// 0x6b
	{ 0x00,0x41,0x7F,0x40,0x00,0x00,0x00,0x00 },	// 0x6c
	{ 0x00,0x7C,0x04,0x18,0x04,0x78,0x00,0x00 },	// 0x6d
	{ 0x00,0x7C,0x08,0x04,0x7C,0x00,0x00,0x00 },	// 0x6e
	{ 0x00,0x38,0x44,0x44,0x38,0x00,0x00,0x00 },	// 0x6f
	{ 0x00,0xFC,0x24,0x24,0x18,0x00,0x00,0x00 },	// 0x70
	{ 0x00,0x18,0x24,0x24,0xFC,0x00,0x00,0x00 },	// 0x71
	{ 0x00,0x00,0x7C,0x08,0x04,0x00,0x00,0x00 },	// 0x72
	{ 0x00,0x48,0x54,0x54,0x24,0x00,0x00,0x00 },	// 0x73
	{ 0x00,0x04,0x7F,0x44,0x00,0x00,0x00,0x00 },	// 0x74
	{ 0x00,0x3C,0x40,0x40,0x7C,0x00,0x00,0x00 },	// 0x75
	{ 0x00,0x1C,0x20,0x40,0x20,0x1C,0x00,0x00 },	// 0x76
	{ 0x00,0x3C,0x40,0x30,0x40,0x3C,0x00,0x00 },	// 0x77
	{ 0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00 },	// 0x78
	{ 0x00,0x1C,0xA0,0xA0,0x7C,0x00,0x00,0x00 },	// 0x79
	{ 0x00,0x44,0x64,0x54,0x4C,0x44,0x00,0x00 },	// 0x7a
	{ 0x00,0x08,0x36,0x41,0x00,0x00,0x00,0x00 },	// 0x7b
	{ 0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00 },	// 0x7c
	{ 0x00,0x41,0x36,0x08,0x00,0x00,0x00,0x00 },	// 0x7d
	{ 0x00,0x02,0x01,0x01,0x02,0x01,0x00,0x00 },	// 0x7e
	{ 0x00,0x02,0x05,0x05,0x02,0x00,0x00,0x00 } 	// 0x7f
};

void GroveOLEDDisplay096::SendCommand(uint8_t cmd)
{
	_Device->WriteReg8(COMMAND_MODE, cmd);
}

void GroveOLEDDisplay096::SendData(uint8_t data)
{
	_Device->WriteReg8(DATA_MODE, data);
}

void GroveOLEDDisplay096::SetHorizontalMode()
{
	SendCommand(0x20);	// Horizontal Addressing Mode
	SendCommand(0x00);	// Set Lower Column Start Address for Page Addressing Mode
}

void GroveOLEDDisplay096::SetPageMode()
{
	SendCommand(0x20);	// Horizontal Addressing Mode
	SendCommand(0x02);	// Set Lower Column Start Address for Page Addressing Mode
}

void GroveOLEDDisplay096::Init()
{
	SendCommand(DISPLAY_OFF_CMD);
	HalSystem::DelayMs(5);
	SendCommand(DISPLAY_ON_CMD);
	HalSystem::DelayMs(5);
	SendCommand(NORMAL_DISPLAY_CMD);
	SetPageMode();
}

void GroveOLEDDisplay096::Clear()
{
	SendCommand(DISPLAY_OFF_CMD);
	for (int row = 0; row < 8; row++)
	{
		SetTextPosition(row, 0);
		{
			for (int column = 0; column < 16; column++)
			{
				write(' ');
			}
		}
	}
	SendCommand(DISPLAY_ON_CMD);
	SetTextPosition(0, 0);
}

void GroveOLEDDisplay096::SetTextPosition(int row, int column)
{
	SendCommand(0xb0 + row);						// Set Page Start Address for Page Addressing Mode
	SendCommand(0x00 + ((8 * column >> 0) & 0x0f));	// Set Lower Column Start Address for Page Addressing Mode
	SendCommand(0x10 + ((8 * column >> 4) & 0x0f));	// Set Higher Column Start Address for Page Addressing Mode
}

void GroveOLEDDisplay096::PrintBitmap(const uint8_t* bitmap, int bitmapSize)
{
	SetHorizontalMode();

	for (int i = 0; i < bitmapSize; i++)
	{
		SendData(bitmap[i]);
	}

	SetPageMode();
}

size_t GroveOLEDDisplay096::write(uint8_t val)
{
	if (val < 0x20 || 0x7f < val) val = ' ';

	for (int i = 0; i < 8; i++)
	{
		SendData(BasicFont[val - 0x20][i]);
	}

	return 1;
}
