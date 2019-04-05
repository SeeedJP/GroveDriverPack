﻿#include "GroveDigitalLight.h"
#include "../HAL/Hal.h"

////////////////////////////////////////////////////////////////////////////////
// Calculation code from TSL2561 data sheet

//****************************************************************************
//
// Copyright  2004−2005 TAOS, Inc.
//
// THIS CODE AND INFORMATION IS PROVIDED ”AS IS” WITHOUT WARRANTY OF ANY
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// PURPOSE.
//
// Module Name:
// lux.cpp
//
//****************************************************************************

#define LUX_SCALE 14 // scale by 2^14
#define RATIO_SCALE 9 // scale ratio by 2^9

//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Integration time scaling factors
//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−

#define CH_SCALE 10 // scale channel values by 2^10
#define CHSCALE_TINT0 0x7517 // 322/11 * 2^CH_SCALE
#define CHSCALE_TINT1 0x0fe7 // 322/81 * 2^CH_SCALE

//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// T, FN, and CL Package coefficients
//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// For Ch1/Ch0=0.00 to 0.50
// Lux/Ch0=0.0304−0.062*((Ch1/Ch0)^1.4)
// piecewise approximation
// For Ch1/Ch0=0.00 to 0.125:
// Lux/Ch0=0.0304−0.0272*(Ch1/Ch0)
//
// For Ch1/Ch0=0.125 to 0.250:
// Lux/Ch0=0.0325−0.0440*(Ch1/Ch0)
//
// For Ch1/Ch0=0.250 to 0.375:
// Lux/Ch0=0.0351−0.0544*(Ch1/Ch0)
//
// For Ch1/Ch0=0.375 to 0.50:
// Lux/Ch0=0.0381−0.0624*(Ch1/Ch0)
//
// For Ch1/Ch0=0.50 to 0.61:
// Lux/Ch0=0.0224−0.031*(Ch1/Ch0)
//
// For Ch1/Ch0=0.61 to 0.80:
// Lux/Ch0=0.0128−0.0153*(Ch1/Ch0)
//
// For Ch1/Ch0=0.80 to 1.30:
// Lux/Ch0=0.00146−0.00112*(Ch1/Ch0)
//
// For Ch1/Ch0>1.3:
// Lux/Ch0=0
//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
#define K1T 0x0040 // 0.125 * 2^RATIO_SCALE
#define B1T 0x01f2 // 0.0304 * 2^LUX_SCALE
#define M1T 0x01be // 0.0272 * 2^LUX_SCALE

#define K2T 0x0080 // 0.250 * 2^RATIO_SCALE
#define B2T 0x0214 // 0.0325 * 2^LUX_SCALE
#define M2T 0x02d1 // 0.0440 * 2^LUX_SCALE

#define K3T 0x00c0 // 0.375 * 2^RATIO_SCALE
#define B3T 0x023f // 0.0351 * 2^LUX_SCALE
#define M3T 0x037b // 0.0544 * 2^LUX_SCALE

#define K4T 0x0100 // 0.50 * 2^RATIO_SCALE
#define B4T 0x0270 // 0.0381 * 2^LUX_SCALE
#define M4T 0x03fe // 0.0624 * 2^LUX_SCALE
#define K5T 0x0138 // 0.61 * 2^RATIO_SCALE
#define B5T 0x016f // 0.0224 * 2^LUX_SCALE
#define M5T 0x01fc // 0.0310 * 2^LUX_SCALE

#define K6T 0x019a // 0.80 * 2^RATIO_SCALE
#define B6T 0x00d2 // 0.0128 * 2^LUX_SCALE
#define M6T 0x00fb // 0.0153 * 2^LUX_SCALE

#define K7T 0x029a // 1.3 * 2^RATIO_SCALE
#define B7T 0x0018 // 0.00146 * 2^LUX_SCALE
#define M7T 0x0012 // 0.00112 * 2^LUX_SCALE

#define K8T 0x029a // 1.3 * 2^RATIO_SCALE
#define B8T 0x0000 // 0.000 * 2^LUX_SCALE
#define M8T 0x0000 // 0.000 * 2^LUX_SCALE

//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// CS package coefficients
//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// For 0 <= Ch1/Ch0 <= 0.52
// Lux/Ch0 = 0.0315−0.0593*((Ch1/Ch0)^1.4)
// piecewise approximation
// For 0 <= Ch1/Ch0 <= 0.13
// Lux/Ch0 = 0.0315−0.0262*(Ch1/Ch0)
// For 0.13 <= Ch1/Ch0 <= 0.26
// Lux/Ch0 = 0.0337−0.0430*(Ch1/Ch0)
// For 0.26 <= Ch1/Ch0 <= 0.39
// Lux/Ch0 = 0.0363−0.0529*(Ch1/Ch0)
// For 0.39 <= Ch1/Ch0 <= 0.52
// Lux/Ch0 = 0.0392−0.0605*(Ch1/Ch0)
// For 0.52 < Ch1/Ch0 <= 0.65
// Lux/Ch0 = 0.0229−0.0291*(Ch1/Ch0)
// For 0.65 < Ch1/Ch0 <= 0.80
// Lux/Ch0 = 0.00157−0.00180*(Ch1/Ch0)
// For 0.80 < Ch1/Ch0 <= 1.30
// Lux/Ch0 = 0.00338−0.00260*(Ch1/Ch0)
// For Ch1/Ch0 > 1.30
// Lux = 0
//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
#define K1C 0x0043 // 0.130 * 2^RATIO_SCALE
#define B1C 0x0204 // 0.0315 * 2^LUX_SCALE
#define M1C 0x01ad // 0.0262 * 2^LUX_SCALE

#define K2C 0x0085 // 0.260 * 2^RATIO_SCALE
#define B2C 0x0228 // 0.0337 * 2^LUX_SCALE
#define M2C 0x02c1 // 0.0430 * 2^LUX_SCALE

#define K3C 0x00c8 // 0.390 * 2^RATIO_SCALE
#define B3C 0x0253 // 0.0363 * 2^LUX_SCALE
#define M3C 0x0363 // 0.0529 * 2^LUX_SCALE

#define K4C 0x010a // 0.520 * 2^RATIO_SCALE
#define B4C 0x0282 // 0.0392 * 2^LUX_SCALE
#define M4C 0x03df // 0.0605 * 2^LUX_SCALE

#define K5C 0x014d // 0.65 * 2^RATIO_SCALE
#define B5C 0x0177 // 0.0229 * 2^LUX_SCALE
#define M5C 0x01dd // 0.0291 * 2^LUX_SCALE

#define K6C 0x019a // 0.80 * 2^RATIO_SCALE
#define B6C 0x0101 // 0.0157 * 2^LUX_SCALE
#define M6C 0x0127 // 0.0180 * 2^LUX_SCALE

#define K7C 0x029a // 1.3 * 2^RATIO_SCALE
#define B7C 0x0037 // 0.00338 * 2^LUX_SCALE
#define M7C 0x002b // 0.00260 * 2^LUX_SCALE

#define K8C 0x029a // 1.3 * 2^RATIO_SCALE
#define B8C 0x0000 // 0.000 * 2^LUX_SCALE
#define M8C 0x0000 // 0.000 * 2^LUX_SCALE

// lux equation approximation without floating point calculations
//////////////////////////////////////////////////////////////////////////////
// Routine: unsigned int CalculateLux(unsigned int ch0, unsigned int ch0, int iType)
//
// Description: Calculate the approximate illuminance (lux) given the raw
// channel values of the TSL2560. The equation if implemented
// as a piece−wise linear approximation.
//
// Arguments: unsigned int iGain − gain, where 0:1X, 1:16X
// unsigned int tInt − integration time, where 0:13.7mS, 1:100mS, 2:402mS,
// 3:Manual
// unsigned int ch0 − raw channel value from channel 0 of TSL2560
// unsigned int ch1 − raw channel value from channel 1 of TSL2560
// unsigned int iType − package type (T or CS)
//
// Return: unsigned int − the approximate illuminance (lux)
//
//////////////////////////////////////////////////////////////////////////////
static uint16_t CalculateLux(int iGain, int tInt, uint16_t ch0, uint16_t ch1, int iType)
{
	//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
	// first, scale the channel values depending on the gain and integration time
	// 16X, 402mS is nominal.
	// scale if integration time is NOT 402 msec
	unsigned long chScale;
	unsigned long channel1;
	unsigned long channel0;
	switch (tInt)
	{
	case 0: // 13.7 msec
		chScale = CHSCALE_TINT0;
		break;
	case 1: // 101 msec
		chScale = CHSCALE_TINT1;
		break;
	default: // assume no scaling
		chScale = (1 << CH_SCALE);
		break;
	}

	// scale if gain is NOT 16X
	if (!iGain) chScale = chScale << 4; // scale 1X to 16X

	// scale the channel values
	channel0 = (ch0 * chScale) >> CH_SCALE;
	channel1 = (ch1 * chScale) >> CH_SCALE;

	//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
	// find the ratio of the channel values (Channel1/Channel0)
	// protect against divide by zero
	unsigned long ratio1 = 0;
	if (channel0 != 0) ratio1 = (channel1 << (RATIO_SCALE + 1)) / channel0;

	// round the ratio value
	unsigned long ratio = (ratio1 + 1) >> 1;

	// is ratio <= eachBreak ?
	uint16_t b, m;
	switch (iType)
	{
	case 0: // T, FN and CL package
		if ((ratio >= 0) && (ratio <= K1T))
		{
			b = B1T; m = M1T;
		}
		else if (ratio <= K2T)
		{
			b = B2T; m = M2T;
		}
		else if (ratio <= K3T)
		{
			b = B3T; m = M3T;
		}
		else if (ratio <= K4T)
		{
			b = B4T; m = M4T;
		}
		else if (ratio <= K5T)
		{
			b = B5T; m = M5T;
		}
		else if (ratio <= K6T)
		{
			b = B6T; m = M6T;
		}
		else if (ratio <= K7T)
		{
			b = B7T; m = M7T;
		}
		else if (ratio > K8T)
		{
			b = B8T; m = M8T;
		}
		break;

	case 1:// CS package
		if ((ratio >= 0) && (ratio <= K1C))
		{
			b = B1C; m = M1C;
		}
		else if (ratio <= K2C)
		{
			b = B2C; m = M2C;
		}
		else if (ratio <= K3C)
		{
			b = B3C; m = M3C;
		}
		else if (ratio <= K4C)
		{
			b = B4C; m = M4C;
		}
		else if (ratio <= K5C)
		{
			b = B5C; m = M5C;
		}
		else if (ratio <= K6C)
		{
			b = B6C; m = M6C;
		}
		else if (ratio <= K7C)
		{
			b = B7C; m = M7C;
		}
		else if (ratio > K8C)
		{
			b = B8C; m = M8C;
		}
		break;
	}

	unsigned long temp;
	temp = ((channel0 * b) - (channel1 * m));

	// do not allow negative lux value
	if (temp < 0) temp = 0;

	// round lsb (2^(LUX_SCALE−1))
	temp += (1 << (LUX_SCALE-1));

	// strip off fractional portion
	unsigned long lux = temp >> LUX_SCALE;

	return(lux);
}

////////////////////////////////////////////////////////////////////////////////

#define TSL2561_CONTROL		(0x80)
#define TSL2561_TIMING		(0x81)
#define TSL2561_INTERRUPT	(0x86)
#define TSL2561_DATA0LOW	(0x8C)
#define TSL2561_DATA0HIGH	(0x8D)
#define TSL2561_DATA1LOW	(0x8E)
#define TSL2561_DATA1HIGH	(0x8F)

void GroveDigitalLight::GetAdcValues(uint16_t* ch0, uint16_t* ch1)
{
	uint8_t low;
	uint8_t high;

	_Device->ReadReg8(TSL2561_DATA0LOW, &low);
	_Device->ReadReg8(TSL2561_DATA0HIGH, &high);
	*ch0 = (high << 8) | low;

	_Device->ReadReg8(TSL2561_DATA1LOW, &low);
	_Device->ReadReg8(TSL2561_DATA1HIGH, &high);
	*ch1 = (high << 8) | low;
}

void GroveDigitalLight::Init()
{
	_Device->WriteReg8(TSL2561_CONTROL, 0x03);	// POWER UP
	_Device->WriteReg8(TSL2561_TIMING, 0x00);	// No High Gain (1x), integration time of 13ms
	_Device->WriteReg8(TSL2561_INTERRUPT, 0x00);
	_Device->WriteReg8(TSL2561_CONTROL, 0x00);	// POWER Down
}

void GroveDigitalLight::Read()
{
	_Device->WriteReg8(TSL2561_CONTROL, 0x03);	// POWER UP
	HalSystem::DelayMs(14);
	uint16_t ch0;
	uint16_t ch1;
	GetAdcValues(&ch0, &ch1);

	_Device->WriteReg8(TSL2561_CONTROL, 0x00);	// POWER Down
	Lux = CalculateLux(0, 0, ch0, ch1, 0);  // T package, no gain, 13ms
}
