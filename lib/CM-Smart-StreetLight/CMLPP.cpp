// Adapted from https://developer.mbed.org/teams/myDevicesIoT/code/Cayenne-LPP/

// Copyright © 2017 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include "CMLPP.h"

CMLPP::CMLPP(uint8_t size) : maxsize(size)
{
  buffer = (uint8_t *)malloc(size);
  cursor = 0;
}

CMLPP::~CMLPP(void)
{
  free(buffer);
}

void CMLPP::reset(void)
{
  cursor = 0;
}

uint8_t CMLPP::getSize(void)
{
  return cursor;
}

uint8_t *CMLPP::getBuffer(void)
{
  //    uint8_t[cursor] result;
  //    memcpy(result, buffer, cursor);
  //    return result;
  return buffer;
}

uint8_t CMLPP::copy(uint8_t *dst)
{
  memcpy(dst, buffer, cursor);
  return cursor;
}

uint8_t CMLPP::addDigitalInput(uint8_t value)
{
  if ((cursor + LPP_DIGITAL_INPUT_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_DIGITAL_INPUT;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addDigitalOutput(uint8_t value)
{
  if ((cursor + LPP_DIGITAL_OUTPUT_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_DIGITAL_OUTPUT;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addAnalogInput(float value)
{
  if ((cursor + LPP_ANALOG_INPUT_SIZE) > maxsize)
  {
    return 0;
  }

  int16_t val = value * 100;
  buffer[cursor++] = LPP_ANALOG_INPUT;
  buffer[cursor++] = val >> 8;
  buffer[cursor++] = val;

  return cursor;
}

uint8_t CMLPP::addAnalogOutput(float value)
{
  if ((cursor + LPP_ANALOG_OUTPUT_SIZE) > maxsize)
  {
    return 0;
  }
  int16_t val = value * 100;
  buffer[cursor++] = LPP_ANALOG_OUTPUT;
  buffer[cursor++] = val >> 8;
  buffer[cursor++] = val;

  return cursor;
}

uint8_t CMLPP::addLightControl(uint8_t value)
{
  if ((cursor + LPP_LIGHT_CONTROL_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_LIGHT_CONTROL;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addColor(uint16_t value)
{
  if ((cursor + LPP_COLOR_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_COLOR;
  buffer[cursor++] = value >> 8;
  buffer[cursor++] = value;

  return cursor;
}
/////////////////////////kammanit//////////////////////////////
uint8_t CMLPP::addError(uint8_t value)
{
  if ((cursor + LPP_ERROR_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_ERROR;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addGen(uint8_t value)
{
  if ((cursor + LPP_GEN_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_GEN;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addModel(uint8_t value)
{
  if ((cursor + LPP_MODEL_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_MODEL;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addRSSI(int8_t value)
{
  if ((cursor + LPP_RSSI_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_RSSI;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addSNR(int8_t value)
{
  if ((cursor + LPP_SNR_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_SNR;
  buffer[cursor++] = value;

  return cursor;
}

/////////////////////////kammanit//////////////////////////////
uint8_t CMLPP::addVoltage(uint16_t value)
{
  if ((cursor + LPP_VOLTAGE_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_VOLTAGE;
  buffer[cursor++] = value >> 8;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addPower(uint16_t value)
{
  if ((cursor + LPP_POWER_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_POWER;
  buffer[cursor++] = value >> 8;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addLuminosity(uint16_t lux)
{
  if ((cursor + LPP_LUMINOSITY_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_LUMINOSITY;
  buffer[cursor++] = lux >> 8;
  buffer[cursor++] = lux;

  return cursor;
}

uint8_t CMLPP::addPresence(uint8_t value)
{
  if ((cursor + LPP_PRESENCE_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_PRESENCE;
  buffer[cursor++] = value;

  return cursor;
}

uint8_t CMLPP::addTemperature(float celsius)
{
  if ((cursor + LPP_TEMPERATURE_SIZE) > maxsize)
  {
    return 0;
  }
  int16_t val = celsius * 10;
  buffer[cursor++] = LPP_TEMPERATURE;
  buffer[cursor++] = val >> 8;
  buffer[cursor++] = val;

  return cursor;
}

uint8_t CMLPP::addRelativeHumidity(float rh)
{
  if ((cursor + LPP_RELATIVE_HUMIDITY_SIZE) > maxsize)
  {
    return 0;
  }
  buffer[cursor++] = LPP_RELATIVE_HUMIDITY;
  buffer[cursor++] = rh * 2;

  return cursor;
}

uint8_t CMLPP::addActuation(uint32_t minute)
{
  if ((cursor + LPP_ACTUATION_SIZE) > maxsize)
  {
    return 0;
  }

  buffer[cursor++] = LPP_ACTUATION;
  buffer[cursor++] = minute >> 40;
  buffer[cursor++] = minute >> 32;
  buffer[cursor++] = minute >> 24;
  buffer[cursor++] = minute >> 16;
  buffer[cursor++] = minute >> 8;
  buffer[cursor++] = minute;

  return cursor;
}

uint8_t CMLPP::addAccelerometer(float x, float y, float z)
{
  if ((cursor + LPP_ACCELEROMETER_SIZE) > maxsize)
  {
    return 0;
  }
  int16_t vx = x * 1000;
  int16_t vy = y * 1000;
  int16_t vz = z * 1000;

  buffer[cursor++] = LPP_ACCELEROMETER;
  buffer[cursor++] = vx >> 8;
  buffer[cursor++] = vx;
  buffer[cursor++] = vy >> 8;
  buffer[cursor++] = vy;
  buffer[cursor++] = vz >> 8;
  buffer[cursor++] = vz;

  return cursor;
}

uint8_t CMLPP::addBarometricPressure(float hpa)
{
  if ((cursor + LPP_BAROMETRIC_PRESSURE_SIZE) > maxsize)
  {
    return 0;
  }
  int16_t val = hpa * 10;

  buffer[cursor++] = LPP_BAROMETRIC_PRESSURE;
  buffer[cursor++] = val >> 8;
  buffer[cursor++] = val;

  return cursor;
}

uint8_t CMLPP::addGyrometer(float x, float y, float z)
{
  if ((cursor + LPP_GYROMETER_SIZE) > maxsize)
  {
    return 0;
  }
  int16_t vx = x * 100;
  int16_t vy = y * 100;
  int16_t vz = z * 100;

  buffer[cursor++] = LPP_GYROMETER;
  buffer[cursor++] = vx >> 8;
  buffer[cursor++] = vx;
  buffer[cursor++] = vy >> 8;
  buffer[cursor++] = vy;
  buffer[cursor++] = vz >> 8;
  buffer[cursor++] = vz;

  return cursor;
}

uint8_t CMLPP::addGPS(float latitude, float longitude, float meters)
{
  if ((cursor + LPP_GPS_SIZE) > maxsize)
  {
    return 0;
  }
  int32_t lat = latitude * 10000;
  int32_t lon = longitude * 10000;
  int32_t alt = meters * 100;

  buffer[cursor++] = LPP_GPS;

  buffer[cursor++] = lat >> 16;
  buffer[cursor++] = lat >> 8;
  buffer[cursor++] = lat;
  buffer[cursor++] = lon >> 16;
  buffer[cursor++] = lon >> 8;
  buffer[cursor++] = lon;
  buffer[cursor++] = alt >> 16;
  buffer[cursor++] = alt >> 8;
  buffer[cursor++] = alt;

  return cursor;
}
