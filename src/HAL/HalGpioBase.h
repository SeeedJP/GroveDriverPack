#pragma once

#if defined MODE_INPUT
#undef MODE_INPUT
#undef MODE_OUTPUT
#endif

class HalGpioBase
{
public:
	enum ModeType
	{
		MODE_INPUT,
		MODE_OUTPUT,
#if defined ARDUINO_ARCH_STM32F4 || defined ARDUINO_ARCH_STM32 || defined __MBED__
		MODE_OUTPUT_OPEN_DRAIN,
#endif // ARDUINO_ARCH_STM32F4 || ARDUINO_ARCH_STM32 || __MBED__
	};

private:
	ModeType _Mode;
	bool _Positive;
	bool _Output;

public:
	void Enable(ModeType mode, bool invert = false)
	{
		EnableImplement(mode);
		_Mode = mode;
		_Positive = !invert;
	}

	void Write(bool on)
	{
		WriteImplement(on == _Positive);
		_Output = on;
	}

	bool ReadOutput() const
	{
		return _Output;
	}

	bool Read()
	{
		return ReadImplement() == _Positive;
	}

protected:
	virtual void EnableImplement(ModeType mode) = 0;
	virtual void WriteImplement(bool high) = 0;
	virtual bool ReadImplement() const = 0;

public:
	virtual ~HalGpioBase() {}

};
