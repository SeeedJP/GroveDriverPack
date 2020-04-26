//GROVE_NAME        "Grove - MP3 V3 -Music Player"
//SKU               107020069
//WIKI_URL          http://wiki.seeedstudio.com/Grove-MP3-v3/

#pragma once

#include "Abstract/GroveModule2.h"
#include "../Connector/GroveConnectorUART.h"

class GroveMP3V3 : public GroveModule2
{
public:
	enum STATUS
	{
		STATUS_PLAY,
		STATUS_STOP,
		STATUS_PAUSE,
	};

private:
	HalUART* _UART;

	void WriteCommand(uint8_t commandCode, const uint8_t* parameter, int parameterSize);
	bool ReadReturn(uint8_t* operationCode, uint8_t* returnValue, uint8_t returnValueSize, int timeout = 1000);

public:
	GroveMP3V3(GroveConnectorUART* connector)
	{
		_UART = &connector->UART;
	}

	bool Init();
	void Play(int index);		// index=1-
	void Stop();
	void SetVolume(int volume);	// volume=0-31
	int QueryVolume();			// 0-31
	STATUS QueryStatus();
	int QueryNumberOfMusic();

};
