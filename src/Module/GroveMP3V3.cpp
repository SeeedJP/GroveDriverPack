#include "GroveMP3V3.h"

static constexpr uint8_t CMD_ROOT_INDEX_PLAY_SD             = 0xa2;
static constexpr uint8_t CMD_STOP                           = 0xab;
static constexpr uint8_t CMD_VOLUME_CONTROL                 = 0xae;
static constexpr uint8_t CMD_QUERY_CURRENT_VOLUME_SETTING   = 0xc1;
static constexpr uint8_t CMD_QUERY_CURRENT_OPERATION_STATE  = 0xc2;
static constexpr uint8_t CMD_QUERY_TOTAL_NUMBER_OF_MUSIC_SD = 0xc5;

void GroveMP3V3::WriteCommand(uint8_t commandCode, const uint8_t* parameter, int parameterSize)
{
    uint8_t length = 1 + 1 + parameterSize + 1;
    uint8_t sum = 0;
    sum += length;
    sum += commandCode;
    for (int i = 0; i < parameterSize; ++i) sum += parameter[i];

    _UART->Write(0x7e);
    _UART->Write(length);
    _UART->Write(commandCode);
    for (int i = 0; i < parameterSize; ++i) _UART->Write(parameter[i]);
    _UART->Write(sum);
    _UART->Write(0xef);
}

bool GroveMP3V3::ReadReturn(uint8_t* operationCode, uint8_t* returnValue, uint8_t returnValueSize, int timeout)
{

    int elapsed = 0;
    while (_UART->ReadAvailable() < 1)
    {
        if (elapsed * 10 >= timeout) return false;
        HalSystem::DelayMs(10);
        ++elapsed;
    }
    *operationCode = _UART->Read();

    for (int i = 0; i < returnValueSize; ++i)
    {
        elapsed = 0;
        while (_UART->ReadAvailable() < 1)
        {
            if (elapsed * 10 >= timeout) return false;
            HalSystem::DelayMs(10);
            ++elapsed;
        }
        returnValue[i] = _UART->Read();
    }

    return true;
}

bool GroveMP3V3::Init()
{
    int retry = 0;
    while (true)
    {
        GroveMP3V3::WriteCommand(CMD_QUERY_TOTAL_NUMBER_OF_MUSIC_SD, nullptr, 0);

        uint8_t operationCode;
        uint8_t totalNumberOfFiles[2];
        if (GroveMP3V3::ReadReturn(&operationCode, totalNumberOfFiles, sizeof(totalNumberOfFiles), 100))    // timeout = 100[msec.]
        {
            if (operationCode == CMD_QUERY_TOTAL_NUMBER_OF_MUSIC_SD) break;
        }

        if (retry >= 100) return false; // retry * timeout = 100 * 100 = 10[sec.]
        ++retry;
    }

    _IsExist = true;
    return true;
}

void GroveMP3V3::Play(int index)
{
    if (!_IsExist) return;

    ++index;

    const uint8_t parameter[]{ static_cast<uint8_t>(index / 256), static_cast<uint8_t>(index % 256) };
    GroveMP3V3::WriteCommand(CMD_ROOT_INDEX_PLAY_SD, parameter, sizeof(parameter));

    uint8_t operationCode;
    if (!GroveMP3V3::ReadReturn(&operationCode, nullptr, 0)) HalSystem::Abort();
    if (operationCode != 0x00) HalSystem::Abort();
}

void GroveMP3V3::Stop()
{
    if (!_IsExist) return;

    GroveMP3V3::WriteCommand(CMD_STOP, nullptr, 0);

    uint8_t operationCode;
    if (!GroveMP3V3::ReadReturn(&operationCode, nullptr, 0)) HalSystem::Abort();
    if (operationCode != 0x00) HalSystem::Abort();
}

void GroveMP3V3::SetVolume(int volume)
{
    if (!_IsExist) return;

    if (volume < 0) volume = 0;
    if (31 < volume) volume = 31;

    const uint8_t parameter[]{ static_cast<uint8_t>(volume) };
    GroveMP3V3::WriteCommand(CMD_VOLUME_CONTROL, parameter, sizeof(parameter));

    uint8_t operationCode;
    if (!GroveMP3V3::ReadReturn(&operationCode, nullptr, 0)) HalSystem::Abort();
    if (operationCode != 0x00) HalSystem::Abort();
}

int GroveMP3V3::QueryVolume()
{
    if (!_IsExist) return 0;

    GroveMP3V3::WriteCommand(CMD_QUERY_CURRENT_VOLUME_SETTING, nullptr, 0);

    uint8_t operationCode;
    uint8_t currentVolume;
    if (!GroveMP3V3::ReadReturn(&operationCode, &currentVolume, 1)) HalSystem::Abort();
    if (operationCode != CMD_QUERY_CURRENT_VOLUME_SETTING) HalSystem::Abort();

    return currentVolume;
}

GroveMP3V3::STATUS GroveMP3V3::QueryStatus()
{
    if (!_IsExist) return STATUS_STOP;

    GroveMP3V3::WriteCommand(CMD_QUERY_CURRENT_OPERATION_STATE, nullptr, 0);

    uint8_t operationCode;
    uint8_t currentOperationState;
    if (!GroveMP3V3::ReadReturn(&operationCode, &currentOperationState, 1)) HalSystem::Abort();
    if (operationCode != CMD_QUERY_CURRENT_OPERATION_STATE) HalSystem::Abort();

    switch (currentOperationState)
    {
    case 1:
        return STATUS_PLAY;
    case 2:
        return STATUS_STOP;
    case 3:
        return STATUS_PAUSE;
    default:
        HalSystem::Abort();
    }

    return STATUS_STOP; // Dummy
}

int GroveMP3V3::QueryNumberOfMusic()
{
    if (!_IsExist) return 0;

    GroveMP3V3::WriteCommand(CMD_QUERY_TOTAL_NUMBER_OF_MUSIC_SD, nullptr, 0);

    uint8_t operationCode;
    uint8_t totalNumberOfFiles[2];
    if (!GroveMP3V3::ReadReturn(&operationCode, totalNumberOfFiles, sizeof(totalNumberOfFiles))) HalSystem::Abort();
    if (operationCode != CMD_QUERY_TOTAL_NUMBER_OF_MUSIC_SD) HalSystem::Abort();

    return totalNumberOfFiles[0] * 256 + totalNumberOfFiles[1];
}
