/*

   @Author Civicmedia
   @Date 21/03/2018


*/

#include <rn2903.h>
#include <SoftwareSerial.h>

#define COLOR_TEMP
#define TEMP_MEASUREMENT
#define VOLT_MEASUREMENT

#define led_built_in 8
#define LED_DIMMER   5


#ifdef COLOR_TEMP
#define LED_COLOR    6
#endif

#ifdef VOLT_MEASUREMENT
#define VOLT_SENSE A1
#endif

#ifdef TEMP_MEASUREMENT
//Pin Define
#define TEMP_SENSE A2

//Constant ref.
#define aref_voltage 3.3
#endif


#define loraSerial mySerial
#define debugSerial Serial

SoftwareSerial loraSerial(9, 7); // RX, TX
rn2903 myLora(loraSerial);

// Set your DevAddr, NwkSKey, AppSKey
/*
  const char *devAddr = "260412CE";
  const char *nwkSKey = "8EA250F087F4426F1AB7EC67ABFDDE80";
  const char *appSKey = "0DB1BEF498DF989AD2828B1C61099E42";
*/
//  devAddr         DevEUI
// 79700011     0004A30B002213D2
// 79700012     0004A30B002232B7
// 79700013     0004A30B00225F42

const char *devAddr = "79700013";
const char *nwkSKey = "28AED22B7E1516A609CFABF715884F3D"; //28AED22B7E1516A609CFABF715884F3C
const char *appSKey = "1628AE2B7E15D2A6ABF7CF4F3C158809";

long previousMillis = 0;
long interval = 30000;

void setup()
{
  //output LED pin
  pinMode(led_built_in, OUTPUT);
  led_on();

  //Output LED Dimmer
  pinMode(LED_DIMMER, OUTPUT);
  analogWrite(LED_DIMMER, 127);

#ifdef COLOR_TEMP //Output LED Dimmer
  pinMode(LED_COLOR, OUTPUT);
  analogWrite(LED_COLOR, 127);
#endif;


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
unsigned long currentMillis = 50000;
// the loop routine runs over and over again forever:
void loop()
{

  currentMillis = millis();

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

    debugSerial.print("Temperature : ");
    debugSerial.println(readTemp());

    debugSerial.print("Volt : ");
    debugSerial.println(readVolt());

    led_off();
  }


  String Data = myLora.getRx() ;
  if (Data != "") {
    debugSerial.println("Received downlink: ");

    Data = Data.substring(Data.indexOf(' ', 7) + 1);
    debugSerial.println(Data);
    //debugSerial.println(charToHexDigit(Data[0]));
    //debugSerial.println(charToHexDigit(Data[1]));

    uint8_t pwm_dimmer = (charToHexDigit(Data[0]) * 16) + charToHexDigit(Data[1]);
    analogWrite(LED_DIMMER, 255 - pwm_dimmer);

#ifdef COLOR_TEMP //Output LED Dimmer
    uint8_t pwm_color = (charToHexDigit(Data[2]) * 16) + charToHexDigit(Data[3]);
    analogWrite(LED_COLOR, 255 - pwm_color);
#endif

    Data = "";
  }

}

uint8_t charToHexDigit(char c)
{
  if (c >= 'A')
    return c - 'A' + 10;
  else
    return c - '0';
}

#ifdef TEMP_MEASUREMENT
unsigned int readTemp() {
  byte count = 0;
  int temp_read = 0;
  float tmp, temp;

  while (count < 10) {
    temp_read += analogRead(TEMP_SENSE);
    count++;
    delay(1);
  }
  temp_read = temp_read / 10;
  tmp = (temp_read * aref_voltage) / 1024;
  temp = -(1000 * tmp - 1546) / 8.2;
  temp = temp * 100;
  return temp;
}
#endif

#ifdef VOLT_MEASUREMENT
unsigned int readVolt() {
  byte count = 0;
  unsigned int volt_read = 0;
  float fval;

  while (count < 10) {
    volt_read += analogRead(VOLT_SENSE);
    count++;
    delay(1);
  }
  /*
    volt_read = volt_read / 10;
    volt_read = volt_read * 3;
    fval = volt_read * 1.367;
    volt_read = fval;
  */
  // 25.7
  volt_read = volt_read / 10;
  volt_read = volt_read * 3;
  fval = volt_read * 1.367;
  volt_read = fval;

  return volt_read;
}
#endif

void led_on()
{
  digitalWrite(led_built_in, 1);
}

void led_off()
{
  digitalWrite(led_built_in, 0);
}
