/*
 *
 * @Author Noppadon,kammanit
 * @Date 22/11/2018
 *
 */

#ifndef Murata_h
#define Murata_h

#include "Arduino.h"

class Murata
{
public:Murata(Stream& serial);

String startat();

String sysver();

String sysclass();

String info_devaddr();

String info_nwkskey();

String info_appskey();

String getsnr();

String getrssi();

bool init();

bool initABP(String addr, String AppSKey, String NwkSKey);

String sendRawCommand(String command);

String checkCommand(String command);

String getRx();

bool sendTx(String lpp);

private:Stream& _serial;

String _devAddr = "00:00:00:00";

String _nwkid = "00:00:00:00";

String _nwkskey = "00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00";

String _appskey = "00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00";

String _rxMessenge = "";

};

#endif
