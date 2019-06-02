#include <SoftwareSerial.h>

SoftwareSerial mySerial(9,7); // RX, TX

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("Start AT");
    //reset murata
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(2, HIGH);


  delay(100); //wait for startup message

  mySerial.print("AT\n");
  delay(100);
//  mySerial.print("AT+NJM=0\n");
//  delay(100);
//  mySerial.print("AT+DADDR=6B:74:01:06\n");
//  delay(100);
//  mySerial.print("AT+NWKSKEY=63:69:76:69:63:6D:65:64:69:61:70:68:75:6B:65:74\n");
//  delay(100);
//  mySerial.print("AT+APPSKEY=4C:49:47:48:54:49:4E:47:50:4C:41:54:46:4F:52:4D\n");
//  delay(100);
//  mySerial.print("AT+RX1DL=1000\n");
//  delay(100);
//  mySerial.print("AT+RX2DL=2000\n");
//  delay(100);
//  mySerial.print("AT+JOIN\n");
//  delay(100);
}

void loop()
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}
