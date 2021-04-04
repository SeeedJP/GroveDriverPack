#pragma once

// HAL

#include "HAL/Hal.h"

// Connector

#include "Connector/GroveConnectorAnalogIn.h"
#include "Connector/GroveConnectorDIO.h"
#include "Connector/GroveConnectorI2C.h"
#include "Connector/GroveConnectorUART.h"

// Module

#include "Module/GroveBuzzer.h"
#include "Module/GroveTiltSwitch.h"
#include "Module/GroveButton.h"
#include "Module/GroveLight.h"
#include "Module/GroveAccelerometer16G.h"
//#include "Module/GroveAccelerometer015G.h"
#include "Module/GroveMagneticSwitch.h"
#include "Module/GroveGPS.h"
#include "Module/GroveRotaryAngle.h"
#include "Module/GroveTempHumiSHT31.h"
#include "Module/GroveHighTemp.h"
#include "Module/GroveTempOneWire.h"
#include "Module/GroveLEDBar2.h"
#include "Module/GroveTempHumiBaroBME280.h"
#include "Module/GroveCircularLED.h"
#include "Module/GroveEMGDetector.h"
#include "Module/GroveRelay.h"
#include "Module/GroveDigitalLight.h"
#include "Module/GroveGasO2.h"
#include "Module/GroveTouch.h"
#include "Module/GroveMiniTrackBall.h"
#include "Module/GroveSpeechRecognizer.h"
//#include "Module/GroveAirQuality.h"
#include "Module/GroveI2CColorSensor2.h"
#include "Module/OmronBaro2SMPB02E.h"
#include "Module/GroveTempHumiSHT35.h"
#include "Module/Grove6AxisAccelGyroLSM6DS3.h"
#include "Module/GroveMP3V3.h"
#include "Module/GrovePM25HM3301.h"
#include "Module/GroveSCD30.h"
#include "Module/GrovePIR.h"

#if defined ARDUINO_ARCH_STM32F4 || defined ARDUINO_ARCH_STM32 || defined ARDUINO_ARCH_SAMD

#include "Module/GroveTempHumiDHT11.h"
#include "Module/GroveTempHumiDHT22.h"
#include "Module/GroveUltrasonicRanger.h"
#include "Module/GroveOLEDDisplay096.h"
#include "Module/GroveLCD1602.h"

#endif // ARDUINO_ARCH_STM32F4 || ARDUINO_ARCH_STM32 || ARDUINO_ARCH_SAMD

#if defined ARDUINO_ARCH_STM32F4 || defined ARDUINO_ARCH_STM32 || defined __MBED__

#include "Module/Grove4DigitDisplay.h"

#endif // ARDUINO_ARCH_STM32F4 || ARDUINO_ARCH_STM32 || __MBED__

// Extenson

#include "Extension/WioExtRTC.h"

// Board

#if defined ARDUINO_WIO_LTE
#include "Board/STM32F4_WIO_GPS/GroveBoard.h"
#elif defined ARDUINO_WIO_3G
#include "Board/WIO_3G/GroveBoard.h"
#elif defined ARDUINO_WIO_LTE_M1NB1_BG96
#include "Board/WIO_LTE_M1NB1_BG96/GroveBoard.h"
#elif defined ARDUINO_Seeed_ReButton
#include "Board/Seeed_ReButton/GroveBoard.h"
#elif defined ARDUINO_WIO_TERMINAL
#include "Board/WIO_TERMINAL/GroveBoard.h"
#endif
