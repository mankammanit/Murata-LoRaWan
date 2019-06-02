/*
 *
 * @Author Noppadon
 * @Date 21/03/2018
 *
 */

#include <SoftwareSerial.h>

#define loraSerial softSerial
#define debugSerial Serial

SoftwareSerial loraSerial(9, 7); // RX, TX


void setup()
{
  while (!debugSerial || !loraSerial)
    ;

  debugSerial.begin(9600);
  delay(1000);
  debugSerial.println("Start");
  loraSerial.begin(9600);

          String response = "";

        // Try a maximum of 10 times with a 1 second delay
        for (uint8_t i=0; i<10 && response==""; i++)
        {
                delay(1000);
                loraSerial.write((byte)0x00);
                loraSerial.write(0x55);
                loraSerial.println();
                // we could use sendRawCommand(F("sys get ver")); here
                loraSerial.println("sys get ver");
                response = loraSerial.readStringUntil('\n');
        }
       debugSerial.println(response);
}

void loop()
{
  while (debugSerial.available())
  {
    loraSerial.write(debugSerial.read());
  }

  while (loraSerial.available())
  {
    debugSerial.write(loraSerial.read());
  }
}
