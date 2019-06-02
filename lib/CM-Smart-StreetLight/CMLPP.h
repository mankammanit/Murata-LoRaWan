// Adapted from https://developer.mbed.org/teams/myDevicesIoT/code/Cayenne-LPP/

// Copyright © 2017 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _CMLPP_H_
#define _CMLPP_H_

#include <Arduino.h>

//LPP_BATTERY = // TODO Unsupported in IPSO Smart Object
//LPP_PROXIMITY = // TODO Unsupported in IPSO Smart Object

#define LPP_DIGITAL_INPUT 0         // 1 byte
#define LPP_DIGITAL_OUTPUT 1        // 1 byte
#define LPP_ANALOG_INPUT 2          // 2 bytes, 0.01 signed
#define LPP_ANALOG_OUTPUT 3         // 2 bytes, 0.01 signed
#define LPP_LUMINOSITY 101          // 2 bytes, 1 lux unsigned
#define LPP_PRESENCE 102            // 1 byte, 1
#define LPP_TEMPERATURE 103         // 2 bytes, 0.1°C signed
#define LPP_RELATIVE_HUMIDITY 104   // 1 byte, 0.5% unsigned
#define LPP_ACTUATION   106         // 6 byte, minute
#define LPP_LIGHT_CONTROL 111       // 1 byte 0-100% unsigned
#define LPP_ACCELEROMETER 113       // 2 bytes per axis, 0.001G
#define LPP_BAROMETRIC_PRESSURE 115 // 2 bytes 0.1 hPa Unsigned
#define LPP_GYROMETER 134           // 2 bytes per axis, 0.01 °/s
#define LPP_GPS 136                 // 3 byte lon/lat 0.0001 °, 3 bytes alt 0.01 meter
#define LPP_COLOR 135               // 2 byte 1 cct unsigned
#define LPP_VOLTAGE 116               // 2 byte 1 cct unsigned
#define LPP_POWER 128               // 2 byte 1:1 watt unsigned
/////////////////////////kammanit//////////////////////////////
#define LPP_ERROR 146               // 2 byte 1:1 gen unsigned
#define LPP_GEN 147               // 2 byte 1:1 gen unsigned
#define LPP_MODEL 148              // 2 byte 1:1 watt unsigned
#define LPP_RSSI 149               // 2 byte 1:1 gen unsigned
#define LPP_SNR 150              // 2 byte 1:1 watt unsigned
/////////////////////////kammanit//////////////////////////////

// Data ID + Data Type + Data Size
#define LPP_DIGITAL_INPUT_SIZE 2       // 1 byte
#define LPP_DIGITAL_OUTPUT_SIZE 2      // 1 byte
#define LPP_ANALOG_INPUT_SIZE 3        // 2 bytes, 0.01 signed
#define LPP_ANALOG_OUTPUT_SIZE 3       // 2 bytes, 0.01 signed
#define LPP_LUMINOSITY_SIZE 3          // 2 bytes, 1 lux unsigned
#define LPP_PRESENCE_SIZE 2            // 1 byte, 1
#define LPP_TEMPERATURE_SIZE 3         // 2 bytes, 0.1°C signed
#define LPP_RELATIVE_HUMIDITY_SIZE 2   // 1 byte, 0.5% unsigned
#define LPP_ACTUATION_SIZE    7        // 6 byte, minute
#define LPP_ACCELEROMETER_SIZE 7       // 2 bytes per axis, 0.001G
#define LPP_BAROMETRIC_PRESSURE_SIZE 3 // 2 bytes 0.1 hPa Unsigned
#define LPP_GYROMETER_SIZE 7           // 2 bytes per axis, 0.01 °/s
#define LPP_GPS_SIZE 10                // 3 byte lon/lat 0.0001 °, 3 bytes alt 0.01 meter
#define LPP_LIGHT_CONTROL_SIZE 2       // 1 byte 0-100% unsigned
#define LPP_COLOR_SIZE 3                  // 2 byte 1 cct unsigned
#define LPP_VOLTAGE_SIZE 3               // 2 byte 1 cct unsigned
#define LPP_POWER_SIZE 3               // 2 byte 1:1 watt unsigned
/////////////////////////kammanit//////////////////////////////
#define LPP_ERROR_SIZE 2               // 1 byte
#define LPP_GEN_SIZE 2               // 1 byte
#define LPP_MODEL_SIZE 2               // 1 byte
#define LPP_RSSI_SIZE 3              // 2 byte
#define LPP_SNR_SIZE 3               // 2 byte
/////////////////////////kammanit//////////////////////////////

class CMLPP
{
public:
  CMLPP(uint8_t size);
  ~CMLPP();

  void reset(void);
  uint8_t getSize(void);
  uint8_t *getBuffer(void);
  uint8_t copy(uint8_t *buffer);

  uint8_t addDigitalInput(uint8_t value);
  uint8_t addDigitalOutput(uint8_t value);

  uint8_t addAnalogInput(float value);
  uint8_t addAnalogOutput(float value);

  uint8_t addLuminosity(uint16_t lux);
  uint8_t addPresence(uint8_t value);
  uint8_t addTemperature(float celsius);
  uint8_t addRelativeHumidity(float rh);
  uint8_t addActuation(uint32_t minute);
  uint8_t addAccelerometer(float x, float y, float z);
  uint8_t addBarometricPressure(float hpa);
  uint8_t addGyrometer(float x, float y, float z);
  uint8_t addGPS(float latitude, float longitude, float meters);
  uint8_t addLightControl(uint8_t value);
  uint8_t addColor(uint16_t value);
  uint8_t addVoltage(uint16_t value);
  uint8_t CMLPP::addPower(uint16_t value);
/////////////////////////kammanit//////////////////////////////
  uint8_t addError(uint8_t value);
  uint8_t addGen(uint8_t value);
  uint8_t addModel(uint8_t value);
  uint8_t addRSSI(int8_t value);
  uint8_t addSNR(int8_t value);
/////////////////////////kammanit//////////////////////////////
private:
  uint8_t *buffer;
  uint8_t maxsize;
  uint8_t cursor;
};

#endif
