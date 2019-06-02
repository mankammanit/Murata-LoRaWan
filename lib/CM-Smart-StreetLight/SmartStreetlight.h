/*
*
* @Author Noppadon,kammanit
* @Date 22/11/2018
*
*/

#ifndef SmartStreetlight_h
#define SmartStreetlight_h

#include "Arduino.h"
//####################//
// ลด max วัตต์ต่ำสูงขึ้น  //
// ลด low วัตต์สูงสูงขึ้น  //
//####################//
////////////////////////////////////// 130 watt mw ///////////////////////////////////
// #define max_pwm_130w    245
// #define low_pwm_130w    168
//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// 45 watt mw///////////////////////////////////////
// #define max_pwm_130w    224   // defualt 224
// #define low_pwm_130w    42     // defualt 42
/////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////// 90 watt mw ///////////////////////////////////
// #define max_pwm_130w    238 // defualt 238
// #define low_pwm_130w    154 // defualt 154
//สำหรับถ้าต่ำไป
// #define max_pwm_130w    230 // defualt 238
// #define low_pwm_130w    154 // defualt 154
//สำหรับถ้าสูงเกิน
// #define max_pwm_130w    240 // defualt 238
// #define low_pwm_130w    154 // defualt 154
//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// 130 watt ieven ///////////////////////////////////
// #define max_pwm_130w    235 // defualt 235
// #define low_pwm_130w    157 // defualt 157
//สำหรับถ้าสูงเกิน
// #define max_pwm_130w    233 // defualt 235
// #define low_pwm_130w    157 // defualt 157
/////////////////////////////////////////////////////////////////////////////////////

////////////// 45 watt ieven mosfet euc-060s180dvm ที่ R=6.2kohms, C=1uF///////////////
#define max_pwm_130w    255   // defualt 255
#define low_pwm_130w    42     // defualt 42
/////////////////////////////////////////////////////////////////////////////////////

#define init_pwm_130w    220

#define color_pwm_init    255

class SmartStreetlight
{
public:
  //SmartStreetlightDebug(Stream& serial);
  SmartStreetlight(uint8_t DimPin, uint8_t ColorPin, Stream& serial);
  SmartStreetlight(uint8_t DimPin, uint8_t ColorPin);
  init(uint8_t bright, uint16_t color);
  Brightness(uint8_t bright);
  Color(uint16_t color);
  Debug();

private:
  Stream& _serial;

  uint8_t
  _dimpin, _colorpin;

  uint8_t
  _pwm_dim_prev = init_pwm_130w, /* PWM0 = Brightness 100% */
  _pwm_color_prev = color_pwm_init /* PWM255 = CCT6500K*/
  ;
};


#endif
