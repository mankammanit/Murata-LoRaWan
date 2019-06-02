/*
*
* @Author Noppadon,kammanit
* @Date 22/11/2018
*
*/

#include "Arduino.h"
#include "Murata.h"

extern "C" {
  #include <string.h>
  #include <stdlib.h>
}

/*
@param serial Needs to be an already opened Stream ({Software/Hardware}Serial) to write to and read from.
*/
Murata::Murata(Stream& serial) :
_serial(serial)
{
  _serial.setTimeout(2000);
}


////////////////////////////// function get ///////////////////////////////////
String Murata::startat()
{
  String at = checkCommand(F("AT\n"));
  return at;
}

String Murata::sysver()
{
  String ver = sendRawCommand(F("AT+VER=?\n"));
  return ver;
}

String Murata::sysclass()
{
  String c = sendRawCommand(F("AT+CLASS=?\n"));
  return c;
}

String Murata::info_devaddr()
{
  String d = sendRawCommand(F("AT+DADDR=?\n"));
  return d;
}

String Murata::info_nwkskey()
{
  String n = sendRawCommand(F("AT+NWKSKEY=?\n"));
  return n;
}

String Murata::info_appskey()
{
  String a = sendRawCommand(F("AT+APPSKEY=?\n"));
  return a;
}

String Murata::getsnr()
{
  String snr = sendRawCommand(F("AT+SNR=?\n"));
  return snr;
}

String Murata::getrssi()
{
  String rssi = sendRawCommand(F("AT+RSSI=?\n"));
  return rssi;
}
///////////////////////////// end function get ////////////////////////////////

bool Murata::initABP(String devAddr, String AppSKey, String NwkSKey)
{
  _devAddr = devAddr;
  _appskey = AppSKey;
  _nwkskey = NwkSKey;

  delay(100);
  while(_serial.available())
  _serial.read();

  //Get or Set the Device Class
  sendRawCommand(F("AT+CLASS=C\n"));
  delay(100);

  //Get or Set the Device address
  sendRawCommand("AT+DADDR="+_devAddr);
  _serial.println();
  delay(200);

  //Get or Set the Network Session Key
  sendRawCommand("AT+NWKSKEY="+_nwkskey);
  _serial.println();
  delay(200);

  //Get or Set the Application Session Key
  for(int i=0 ; i<2 ;i++ )
  {
    sendRawCommand("AT+APPSKEY="+_appskey);
    _serial.println();
    delay(200);
  }

  //Disable Dutycycle mode test
  sendRawCommand(F("AT+DCS=0\n"));
  delay(100);

  //Tranmission Power
  //0=min , 5=max
  sendRawCommand(F("AT+TXP=5\n"));
  delay(100);

  //Datarate SF10/125kHz
  //0=min, 7=max
  sendRawCommand(F("AT+DR=2\n"));
  delay(100);

  //Set the Adaptive Data Rate setting. (0: off, 1: on)
  sendRawCommand(F("AT+ADR=1\n"));
  delay(100);

  //Set Join delay of the RX window 1
  sendRawCommand(F("AT+JN1DL=5000\n"));
  delay(100);

  //Set Join delay of the RX window 2
  sendRawCommand(F("AT+JN2DL=6000\n"));
  delay(100);

  //Set delay of the received window 1
  sendRawCommand(F("AT+RX1DL=1000\n"));
  delay(100);

  //Set delay of the received window 2
  sendRawCommand(F("AT+RX2DL=2000\n"));
  delay(100);

  //Get or Set the Network Join Mode. (0: ABP, 1: OTAA)
  sendRawCommand(F("AT+NJM=0\n"));
  delay(100);

  //Check Status Join Network
  String receivedData = sendRawCommand(F("AT+NJS=?\n"));
  delay(200);


  if(receivedData.startsWith("1"))
  {
    return true;
  }
  else
  {
    return false;
  }
}

String Murata::sendRawCommand(String command)
{
  delay(100);
  while(_serial.available())
  _serial.read();
  _serial.print(command);
  String ret = _serial.readStringUntil('\n');
  return ret;
}

String Murata::checkCommand(String command)
{
  delay(100);
  while(_serial.available())
  _serial.read();
  _serial.print(command);
  String retstatus = _serial.readString();
  return retstatus;
}

bool Murata::sendTx(String lpp) {

  Serial.println(lpp);

  String ret = "";
  String retstatus = "";

  bool send_success = false;
  uint8_t retry_count = 0;

  delay(100);
  //clear buffer
  while (_serial.available())
  _serial.read();

  uint8_t port = random(1, 223);
  String portrandom = String(port);

  while(!send_success)
  {
    //retransmit a maximum of 10 times
    retry_count++;
    Serial.print(F("SEND COUNT : "));
    Serial.println(retry_count);

    if(retry_count>10)
    {
      Serial.println(F("## SEND FAILED COUNT > 10 ##"));
      send_success = true;
      return false;
    }
    else
    {
      _serial.print("AT+SENDB=" + portrandom);
      _serial.print(":"+ lpp);
      _serial.println();
      ret = _serial.readStringUntil('\n');
      // Serial.print(F("SEND LPP : "));
      // Serial.println(ret);
      retstatus = _serial.readStringUntil('\n');
      // Serial.print(F("SEND STATUS : "));
      // Serial.println(retstatus);
    }

    if (retstatus.startsWith("OK"))
    {
      send_success = true;
      Serial.println(F("## SEND SUCCESS ##"));
      return true;
    }
    else
    {
      //retry send 1 seconds
      Serial.println(F("## SEND FAILED ##"));
      delay(1000);
    }
  }
  return false;
}



String Murata::getRx() {

  String _rxMessenge = "";
  String _rxMessenge2 = "";

  delay(100);
  //clear buffer
  while(_serial.available())
  _serial.read();

  _serial.print("AT+RECVB=?\n");

  //RECEV MESSAGE
  _rxMessenge = _serial.readStringUntil('\n');
  // Serial.println(_rxMessenge);
  if(_rxMessenge.length() > 3) {
    // Serial.println("length>3");
    // Serial.println(_rxMessenge);
    _rxMessenge2 = _serial.readStringUntil('\n');
    _rxMessenge2 = _serial.readStringUntil('\n');
    // Serial.println(_rxMessenge2);

    if (_rxMessenge2.startsWith("OK"))

    {
      if (_rxMessenge.substring(1, 2) == ":")
      { //    ## String 1 port ##
        _rxMessenge = _rxMessenge.substring(2);

        if(_rxMessenge.length() > 1)
        return _rxMessenge;
        else
        return "FAILED";
      }
      else if (_rxMessenge.substring(2, 3) == ":")
      {
        //   ## String 2 port ##
        _rxMessenge = _rxMessenge.substring(3);
        if(_rxMessenge.length() > 1)
        return _rxMessenge;
        else
        return "FAILED";

      }
      else if (_rxMessenge.substring(3, 4) == ":")
      {
        //  ## String 3 port ##
        _rxMessenge = _rxMessenge.substring(4);
        if(_rxMessenge.length() > 1)
        return _rxMessenge;
        else
        return "FAILED";

      }
      else
      {
        return "FAILED";
      }
    }
  }
  return "FAILED";
}
