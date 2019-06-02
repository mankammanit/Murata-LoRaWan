#include <Murata.h>
#include <SoftwareSerial.h>
#include <TaskScheduler.h>
#include <SmartStreetlight.h>
#include <ByteConvert.h>
#include <CMLPP.h>
#include <tool.h>

//////////////////////// ตั้งค่าไอดีโคม /////////////////////////////////////
const char *devAddr = "00:00:00:00";
///สำหรับทดสอบ///
// const char *nwkSKey = "28:AE:D2:2B:7E:15:16:A6:09:CF:AB:F7:15:88:4F:3C";
// const char *appSKey = "16:28:AE:2B:7E:15:D2:A6:AB:F7:CF:4F:3C:15:88:09";
///สำหรับงานภูเก็ต///
const char *nwkSKey = "63:69:76:69:63:6D:65:64:69:61:70:68:75:6B:65:74";
const char *appSKey = "4C:49:47:48:54:49:4E:47:50:4C:41:54:46:4F:52:4D";
///สำหรับงานแพรกษา///
// const char *nwkSKey = "63:69:76:69:63:6D:65:64:69:61:50:52:41:4B:53:41";
// const char *appSKey = "4C:49:47:48:54:49:4E:47:50:4C:41:54:46:4F:52:4D";
///สำหรับงานอุดร///
// const char *nwkSKey = "43:49:56:49:43:4D:45:44:49:41:55:44:4F:4E:54:48";
// const char *appSKey = "4C:49:47:48:54:49:4E:47:50:4C:41:54:46:4F:52:4D";
///สำหรับงานกระทุ่มล้ม///
// const char *nwkSKey = "43:49:56:49:43:4D:45:44:49:41:4B:41:54:48:55:4D";
// const char *appSKey = "4C:49:47:48:54:49:4E:47:50:4C:41:54:46:4F:52:4D";
///////////////////////////////////////////////////////////////////////
//////////////////////// kammanit set gen model ///////////////////////
//Dim 10 - 100% & Adj CCT 2700-6500 , 2 = Dim 10 - 100% only & not Adj CCT
uint8_t set_gen = 1;
//1 = 45watt , 2 = 70watt , 3 = 130watt , 4 = 130watt , 5 = 90watt
uint8_t set_model = 1;
//////////////////////////////////////////////////////////////////////
////////////////////// ตั้งค่า send data 10 min /////////////////////////
unsigned long previousMillis = 0;
unsigned long interval = 60000*10;
////////////////////////////////////////////////////////////////////

bool send_data_tx = false;

////// PIN OUT LED //////
#define led_built_in 8
#define LED_DIMMER   5
#define LED_COLOR    6
/////////////////////////

#define loraSerial mySerial
#define debugSerial Serial
#define FS(x) (__FlashStringHelper*)(x)

SoftwareSerial loraSerial(9, 7); // RX, TX ใช้ 9,7
Murata myLora(loraSerial);
SmartStreetlight myStreetlight(LED_DIMMER, LED_COLOR, debugSerial);

CMLPP lpp(51);

Scheduler Tasks;
void LEDOSCallback();
bool LED = 0;
Task LEDOS(1000, TASK_FOREVER, &LEDOSCallback);

int num = 0;
uint16_t dimmer = 100;
uint16_t colortemp = 6500;
uint32_t worktime = 0;
uint8_t counter_sec = 0;
uint8_t retry_send = 0;

//////////////////// บันทึก EEPROM ID STREET LIGHT ////////////////////
char save_id[11];
char dev_id[11];
String val , sub_val;
String id_sl  = "00:00:00:00";
/////////////////////////////////////////////////////////////////////

void setup()
{

  if (loadConfig()) {

    dimmer = storage.brightness;
    colortemp = storage.cct;
    worktime = storage.wt;
    myStreetlight.init(dimmer, colortemp);

  } else saveConfig(); //initial config


  // Open serial communications and wait for port to open:
  debugSerial.begin(57600); //serial port to computer
  loraSerial.begin(9600); //serial port to radio

  ///////////// Check Factory //////////////////
  CALL_DADDR();
  /////////////////////////////////////////////
  if(strcmp(dev_id, "00:00:00:00") == 0)
  {
    debugSerial.println(F("#####################################"));
    debugSerial.println(F("MODE SETUP"));
    debugSerial.println(F("#####################################"));
    while(1)
    {
      SET_DADDR_VB(); //ตั้งค่าไอดีโคม
    }
  }
  else
  {
    debugSerial.println(F("#####################################"));
    debugSerial.println(F("MODE WORKING"));
    debugSerial.println(F("#####################################"));
    debugSerial.println(F("Startup"));
    pinMode(led_built_in, OUTPUT);
    Tasks.init();
    Tasks.addTask(LEDOS);
    LEDOS.enable();
    initialize_radio();
  }

}


// the loop routine runs over and over again forever:
void loop()
{
  while(retry_send<5)
  {
    retry_send++;
    UPLINK();
    delay(1000);
  }
  SET_DADDR_VB(); //ตั้งค่าไอดีโคม
  Tasks.execute(); //รัน multitasking
  unsigned long currentMillis = millis();
  ///////////////////// ส่วนส่งข้อมูล ///////////////////////////
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    UPLINK();
  }
  ////////////////////////////////////////////////////////////
  ///////////////////// ส่วนรับข้อมูล ///////////////////////////
  else
  {
    DOWNLINK();
  }
  ////////////////////////////////////////////////////////////
}

void UPLINK()
{

  debugSerial.print(F("LOOP : "));
  debugSerial.println(num++);

  lpp.reset();
  lpp.addTemperature(readTemp() / 10.0);
  lpp.addActuation(worktime);
  lpp.addVoltage(readVolt() - 300.00);

  if (set_model == 1)lpp.addPower(dimmer * 45 / 100);
  else if (set_model == 2)lpp.addPower(dimmer * 70 / 100);
  else if (set_model == 3)lpp.addPower(dimmer * 130 / 100);
  else if (set_model == 4)lpp.addPower(dimmer * 130 / 100);
  else if (set_model == 5)lpp.addPower(dimmer * 90 / 100);

  lpp.addLightControl(dimmer);
  lpp.addColor(colortemp);

  if (((readVolt() - 300.00) / 100.00) > 41.00)
  {
    lpp.addError(1);
  }
  else if (((readVolt() - 300.00) / 100.00) < 28.00)
  {
    lpp.addError(2);
  }
  else if ((readTemp() / 10.0) > 80.00)
  {
    lpp.addError(3);
  }
  else
  {
    lpp.addError(0);
  }

  lpp.addGen(set_gen);
  lpp.addModel(set_model);

  // Downlink signal check
  String rssi = myLora.getrssi();
  rssi.remove(0,1); //delete char "-"
  lpp.addRSSI(rssi.toInt());
  lpp.addSNR(myLora.getsnr().toInt());


  // debugSerial.print(F("Voltage : "));
  // debugSerial.println((readVolt() - 300.00) / 100.00);
  // debugSerial.print(F("Temperature : "));
  // debugSerial.println(readTemp() / 10.00);

  send_data_tx = myLora.sendTx(arrayToString(lpp.getSize(), lpp.getBuffer()));

  if (send_data_tx == false)
  {
    eeprom_read_string(100, dev_id, 12);
    devAddr = dev_id;
    delay(500);
    initialize_radio();
    retry_send=0;
    debugSerial.println(F("## Retry Join Network Success ##"));
  }
}

void DOWNLINK()
{
  String DataRx = myLora.getRx();
  uint8_t ptr = 0;
  char Data[11];
  DataRx.toCharArray(Data, 11);
  // Serial.println(DataRx);
  if (DataRx != "FAILED") {
    debugSerial.print(F("Received downlink : "));
    debugSerial.println(Data);
    debugSerial.print(F("RSSI : "));
    debugSerial.print(myLora.getrssi());
    debugSerial.print(F(" , SNR : "));
    debugSerial.println(myLora.getsnr());
    while (ptr < (DataRx.length() - 1)) {
      if (Data[ptr] == '6' && Data[ptr + 1] == 'f' )
      {
        ptr += 2;
        dimmer = htoi(Data[ptr]) * 16 + htoi(Data[ptr + 1]);
        ptr += 2;
        if (dimmer <= 10) dimmer = 10;
        if (dimmer >= 100) dimmer = 100;
        myStreetlight.Brightness(dimmer);
        debugSerial.print(F("Brightness : "));
        debugSerial.println(dimmer);
      }
      else if (Data[ptr] == '8' && Data[ptr + 1] == '7' )
      {
        ptr += 2;
        colortemp = (htoi(Data[ptr]) * 16 + htoi(Data[ptr + 1])) * 256;
        colortemp += htoi(Data[ptr + 2]) * 16 + htoi(Data[ptr + 3]);
        ptr += 4;
        //CCT 2700-6500K
        myStreetlight.Color(colortemp);
        debugSerial.print(F("CCT : "));
        debugSerial.println(colortemp);
      }
      else if (Data[ptr] == '1' && Data[ptr + 1] == '1' )
      {
        pinMode(13, OUTPUT);
        ptr += 2;
        uint8_t testlight = htoi(Data[ptr]) * 16 + htoi(Data[ptr + 1]);
        ptr += 2;
        if(testlight==1)
        {
          debugSerial.println(F("DEMO_LED"));
          digitalWrite(13, HIGH);
          delay(500);
          digitalWrite(13, LOW);
        }
      }
      else
      {
        ptr = ptr + 2;
      }
    }
    storage.brightness = dimmer;
    storage.cct = colortemp;
    saveConfig();
    DataRx = "";
    for ( uint8_t i = 0; i < sizeof(Data);  ++i )
    Data[i] = (char)0;
  }
}


void initialize_radio()
{
  //reset murata
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delay(2000);
  digitalWrite(2, HIGH);

  delay(1000); //wait for startup message
  loraSerial.flush();

  debugSerial.print(F("Connection Murata:"));

  if (myLora.startat() == "\r\nOK\r\n")
  {
    debugSerial.println(F("OK"));
  }
  else
  {
    debugSerial.print(F("FAILED"));
    setup();
  }

  //info Show Configuration MuRata
  debugSerial.print(F("MuRata firmware version:"));
  debugSerial.println(myLora.sysver());

  debugSerial.print(F("Murata Class:"));
  debugSerial.println(myLora.sysclass());

  debugSerial.println(F("Trying to join ABP"));

  bool join_result = false;

  //Join Network ABP
  join_result = myLora.initABP(devAddr, appSKey, nwkSKey);

  while (!join_result)
  {
    debugSerial.println(F("Unable to join. Are your keys correct, and do you have Network coverage?"));
    delay(5000); //delay a minute before retry
    join_result = myLora.initABP(devAddr, appSKey, nwkSKey);
  }

  debugSerial.print(F("Murata DevAddr:"));
  debugSerial.println(myLora.info_devaddr());

  debugSerial.print(F("Murata appSKey:"));
  debugSerial.println(myLora.info_appskey());

  debugSerial.print(F("Murata nwkSKey:"));
  debugSerial.println(myLora.info_nwkskey());

  debugSerial.println(F("Successfully joined Network"));
}

void CALL_DADDR()
{
  byte flag_sl_id = EEPROMReadInt(90);
  if (flag_sl_id == 50)
  {
    eeprom_read_string(100, dev_id, 12);
    devAddr = dev_id;
  }
  else
  {
    id_sl.toCharArray(save_id, 12);
    strcpy(dev_id, save_id);
    eeprom_write_string(100, dev_id);
    eeprom_read_string(100, dev_id, 12);
    EEPROMWriteInt(90, 50);
    EEPROMReadInt(90);
  }
}

void SET_DADDR_VB()
{
  if (debugSerial.available() > 0)
  {
    debugSerial.flush();
    val = debugSerial.readString();
    if (val.substring(0, 2) == "0A") //read id
    {
      debugSerial.println(F("#####################################"));
      debugSerial.print(F("READ ID STREET LIGHT: "));
      eeprom_read_string(100, dev_id, 12);
      debugSerial.println(dev_id);
      debugSerial.println(F("#####################################"));
    }
    else if (val.substring(0, 2) == "0B") //set id
    {
      sub_val = val.substring(2, 13);
      if (sub_val.length() == 11)
      {
        debugSerial.println(F("#####################################"));
        debugSerial.print(F("SET ID STREET LIGHT: "));
        sub_val.toCharArray(save_id, 12);
        strcpy(dev_id, save_id);
        eeprom_write_string(100, dev_id);
        eeprom_read_string(100, dev_id, 12);
        debugSerial.println(dev_id);
        debugSerial.println(F("!! SET ID SUCCESS !!"));
        debugSerial.println(F("#####################################"));
        retry_send=5;
      }
    }
  }
}

void LEDOSCallback() {
  LED = !LED;
  digitalWrite(led_built_in, LED);
  if (counter_sec  == 59) {
    worktime++;
    debugSerial.print(F("Save worktime : "));
    debugSerial.println(worktime);
    storage.wt = worktime;
    saveConfig();
    counter_sec = 0;
  } else {
    counter_sec++;
  }

}
