/*
 *
 * @Author Noppadon,kammanit
 * @Date 22/11/2018
 *
 */

#include "Arduino.h"
#include "SmartStreetlight.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

SmartStreetlight::SmartStreetlight(uint8_t DimPin, uint8_t ColorPin, Stream& serial) :
        _serial(serial)
{
        _serial.setTimeout(2000);

        _dimpin = DimPin;
        _colorpin = ColorPin;

        //Output LED Dimmer
        pinMode(_dimpin, OUTPUT);
        //analogWrite(_dimpin, _pwm_dim_prev);


        pinMode(_colorpin, OUTPUT);
        //analogWrite(_colorpin, color_pwm_init);
}

SmartStreetlight::SmartStreetlight(uint8_t DimPin, uint8_t ColorPin)
{
        _dimpin = DimPin;
        _colorpin = ColorPin;
        pinMode(_dimpin, OUTPUT);
        pinMode(_colorpin, OUTPUT);


}

SmartStreetlight::init(uint8_t bright, uint16_t color)
{
  _pwm_dim_prev = bright;
  _pwm_color_prev = color;

  Brightness(bright);
  Color(color);
}

SmartStreetlight::Brightness(uint8_t bright)
{
        //_serial.print(F("Bright : ")); _serial.print(bright); _serial.print(" PWM : ");
        //  _serial.println(map(bright, 0, 100, 0, 255));
        uint8_t pwm_dimmer = map(bright, 100, 10, low_pwm_130w, max_pwm_130w);
        // _serial.println(pwm_dimmer); //invert 100% = low_pwm_130w, 10% = max_pwm_130w

        while(_pwm_dim_prev != pwm_dimmer) {
                if(_pwm_dim_prev <= pwm_dimmer) { //increase PWM , decrease Brightness

                        //  while(_pwm_dim_prev != pwm_dimmer) {
                      //  _serial.print("increase : "); _serial.print(_pwm_dim_prev);
                        _pwm_dim_prev++;
                        delay(5);
                        analogWrite(_dimpin, _pwm_dim_prev);
                        //  }

                        //  _pwm_dim_prev = pwm_dimmer;

                }else if(_pwm_dim_prev > pwm_dimmer) { // decrease PWM, increase Brightness

                        //    while(_pwm_dim_prev != pwm_dimmer) {
                      //  _serial.print("decrease : "); _serial.print(_pwm_dim_prev);
                        _pwm_dim_prev--;
                        delay(5);
                        analogWrite(_dimpin, _pwm_dim_prev);
                        //    }
                        //    _pwm_dim_prev = pwm_dimmer;
                }

        }
        _pwm_dim_prev = pwm_dimmer;
}

SmartStreetlight::Color(uint16_t color)
{
        //_serial.print("color : "); _serial.print(color); _serial.print(" PWM : ");
        //_serial.println(map(color, 2700, 6500, 0, 255));
        uint8_t pwm_color = map(color, 2700, 6500, 0, 255);
        //_serial.println(pwm_color); // 2700K = PWM0, 6500K = PWM255

        while(_pwm_color_prev != pwm_color) {
                if(_pwm_color_prev <= pwm_color) {         //increase PWM , increase CCT
                        //_serial.print("increase : "); _serial.print(_pwm_color_prev);
                        _pwm_color_prev++;
                        delay(5);
                        analogWrite(_colorpin, _pwm_color_prev);
                }else if(_pwm_color_prev > pwm_color) {         // decrease PWM, decrease CCT
                        //_serial.print("decrease : "); _serial.print(_pwm_color_prev);
                        _pwm_color_prev--;
                        delay(5);
                        analogWrite(_colorpin, _pwm_color_prev);
                }

        }
        _pwm_color_prev = pwm_color;
}
