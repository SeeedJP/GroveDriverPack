#pragma once

#include <mbed.h>

class HalSystem
{
public:
	HalSystem() = delete;

	static void DelayMs(uint32_t milliSeconds)
	{
#ifdef THIS_THREAD_H
		ThisThread::sleep_for(milliSeconds);
#else
		wait_ms(milliSeconds);
#endif // THIS_THREAD_H
	}

	static void DelayUs(uint32_t microSeconds)
	{
		wait_us(microSeconds);
	}

	static uint32_t ClockUs()
	{
		abort();	// TODO
	}

	static void Abort()
	{
		abort();
	}

};
