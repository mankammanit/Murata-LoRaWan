/*

   @Author Civicmedia
   @Date 20/03/2018

*/

#include <rn2903.h>
#include <SoftwareSerial.h>

#define led_built_in 8

#define loraSerial mySerial
#define debugSerial Serial

SoftwareSerial loraSerial(9, 7); // RX, TX
rn2903 myLora(loraSerial);

// Set your DevAddr, NwkSKey, AppSKey
const char *devAddr = "260412CE";
const char *nwkSKey = "8EA250F087F4426F1AB7EC67ABFDDE80";
const char *appSKey = "0DB1BEF498DF989AD2828B1C61099E42";

long previousMillis = 0;
long interval = 30000;

void setup()
{
  //output LED pin
  pinMode(led_built_in, OUTPUT);
  led_on();

  // Open serial communications and wait for port to open:
  debugSerial.begin(9600); //serial port to computer
  loraSerial.begin(9600); //serial port to radio
  debugSerial.println("Startup");

  initialize_radio();

  led_off();
  delay(2000);
}

void initialize_radio()
{
  //reset rn2483
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);

  delay(100); //wait for startup message
  loraSerial.flush();

  //Autobaud the  module Default 57600.
  myLora.autobaud();

  //check communication with radio
  String hweui = myLora.hweui();
  while (hweui.length() != 16)
  {
    debugSerial.println("Communication with RN2903 unsuccesful. Power cycle the board.");
    debugSerial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  //print out the HWEUI so that we can register it via ttnctl
  debugSerial.println("When using OTAA, register this DevEUI: ");
  debugSerial.println(myLora.hweui());
  debugSerial.println("RN2903 firmware version:");
  debugSerial.println(myLora.sysver());
  debugSerial.println("RN2903 Class:");
  debugSerial.println(myLora.sysclass());

  //configure your keys and join the network
  debugSerial.println("Trying to join ABP");
  bool join_result = false;

  //ABP: initABP(String addr, String AppSKey, String NwkSKey);
  join_result = myLora.initABP(devAddr, appSKey, nwkSKey);

  while (!join_result)
  {
    debugSerial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = myLora.init();
  }
  debugSerial.println("Successfully joined TTN");

}

int i = 0;
// the loop routine runs over and over again forever:
void loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    led_on();

    debugSerial.print("LOOP  "); debugSerial.println(i++);

    switch (myLora.txUncnf("abc")) //one byte, blocking function
    {
      case TX_FAIL:
        {
          debugSerial.println("TX unsuccessful or not acknowledged");
          break;
        }
      case TX_SUCCESS:
        {
          debugSerial.println("TX successful and acknowledged");
          break;
        }
      case TX_WITH_RX:
        {
          String received = myLora.getRx();
          received = myLora.base16decode(received);
          debugSerial.print("Received downlink: " + received);
          break;
        }
      default:
        {
          debugSerial.println("Unknown response from TX function");
        }
    }

    led_off();
  }


}

void led_on()
{
  digitalWrite(led_built_in, 1);
}

void led_off()
{
  digitalWrite(led_built_in, 0);
}
