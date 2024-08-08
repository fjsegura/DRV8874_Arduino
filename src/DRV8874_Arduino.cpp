/*
  DRV8874_Arduino.cpp - Library for using DRV8874 from Texas Instruments.
  Created by Francisco Segura, August 8, 2024.
  Updated by Francisco Segura, February 8, 2024.
  Released into the public domain.
*/
#include "Arduino.h"
#include "DRV8874_Arduino.h"

/*
The DRV8874 class is the basis to run a DRV8874 Driver from Texas Instruiments.

It requires 3 control pins, and 1 input pin. Also allows to invert control in case of multiple connections. Supports both PH/EN mode and PWM mode.
For PWM mode both In1 and In2 must be PWM enabled. 
alarmPin must be pullup, either externally or internally. Internal pullup can be enabled in `begin`
*/
DRV8874::DRV8874(
		    int enIn1Pin,
		    int phIn2Pin,
		    int sleepPin,
		    int alarmPin,
		    bool invertControl = false,
		    bool enablePwmMode = false
		    );
{
  _enIn1Pin = enIn1Pin;
  _phIn2Pin = phIn2Pin;
  _sleepPin = sleepPin;
  _alarmPin = alarmPin;
  _enablePwmMode = enablePwmMode;
  _invertControl = invertControl;
}
/*
Begin function
*/
void DRV8874::begin()
{
  pinMode(_pin, OUTPUT);
}
/*
 
 
*/
void DRV8874::dot()
{
  digitalWrite(_pin, HIGH);
  delay(250);
  digitalWrite(_pin, LOW);
  delay(250);  
}

void DRV8874::dash()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}
