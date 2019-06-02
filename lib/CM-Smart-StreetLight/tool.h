#ifndef _tool_h
#define _tool_h

#include "Arduino.h"
#include <EEPROM.h>

#define VOLT_MEASUREMENT
#define TEMP_MEASUREMENT

#define CONFIG_VERSION "ls2"

#define CONFIG_START 0

const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;

struct StoreStruct {
  char version[4];
  uint8_t brightness;
  uint16_t cct;
  uint32_t wt; //worktime
} storage = {
  CONFIG_VERSION,
  100,
  6500,
  0
};

bool loadConfig() {
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
  EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
  EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]){
    for (unsigned int t=0; t<sizeof(storage); t++)
    *((char*)&storage + t) = EEPROM.read(CONFIG_START + t);
    return true;
  }else{
    return false;
  }
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(storage); t++)
  EEPROM.write(CONFIG_START + t, *((char*)&storage + t));
}


#ifdef VOLT_MEASUREMENT

#define VOLT_SENSE A1


unsigned int readVolt() {
  byte count = 0;
  unsigned int volt_read = 0;
  float fval = 0.0;

  while (count < 10) {
    volt_read += analogRead(VOLT_SENSE);
    count++;
    delay(1);
  }

  volt_read = volt_read / 10;
  fval = ((volt_read * 3.3)/1024.0) * 13.0;
  volt_read = fval * 100;

  return volt_read;
}
#endif

#ifdef TEMP_MEASUREMENT

//Pin Define
#define TEMP_SENSE A2

//Constant ref.
#define aref_voltage 3.3


unsigned int readTemp() {
  byte count = 0;
  unsigned int temp_read = 0;
  float tmp, temp;

  while (count < 10) {
    temp_read += analogRead(TEMP_SENSE);
    count++;
    delay(1);
  }

  temp_read = temp_read / 10;
  tmp = ((float)temp_read * aref_voltage) / 1024.0;
  temp = -(1000.0 * tmp - 1546.0) / 8.2;
  temp_read = temp * 10;
  return temp_read;
}
#endif

uint8_t charToHexDigit(char c)
{
  if (c >= 'A')
  return c - 'A' + 10;
  else
  return c - '0';
}

#endif



boolean eeprom_is_addr_ok(int addr) {
return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}


boolean eeprom_write_bytes(int startAddr, const byte* array, int numBytes) {

int i;

if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
return false;
}
for (i = 0; i < numBytes; i++) {
EEPROM.write(startAddr + i, array[i]);
}
return true;
}

boolean eeprom_write_string(int addr, const char* string) {

int numBytes;

numBytes = strlen(string) + 1;
return eeprom_write_bytes(addr, (const byte*)string, numBytes);
}

boolean eeprom_read_string(int addr, char* buffer, int bufSize) {
byte ch;
int bytesRead;
if (!eeprom_is_addr_ok(addr)) {
return false;
}

if (bufSize == 0) {
return false;
}

if (bufSize == 1) {
buffer[0] = 0;
return true;
}
bytesRead = 0;
ch = EEPROM.read(addr + bytesRead);
buffer[bytesRead] = ch;
bytesRead++;

while ( (ch != 0x00) && (bytesRead < bufSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR) ) {

ch = EEPROM.read(addr + bytesRead);
buffer[bytesRead] = ch;
bytesRead++;
}

if ((ch != 0x00) && (bytesRead >= 1)) {
buffer[bytesRead - 1] = 0;
}
return true;
}

void EEPROMWriteInt(int address, int value)
{
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);

  EEPROM.update(address, two);
  EEPROM.update(address + 1, one);
}

int EEPROMReadInt(int address)
{
  long two = EEPROM.read(address);
  long one = EEPROM.read(address + 1);

  return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
}
