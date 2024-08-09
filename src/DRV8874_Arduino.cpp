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
Begin function, enable internal pull with pullupAlarm = true.
*/
void DRV8874::begin(bool pullupAlarm = false)
{
  // Setup pin modes
  pinMode(_enIn1Pin, OUTPUT);
  pinMode(_phIn2Pin, OUTPUT);
  pinMode(_sleepPin, OUTPUT);
  if (pullupAlarm){
  	pinMode(_alarmPin,INPUT_PULLUP);
  } else {
  	pinMode(_alarmPin,INPUT);
  }
}

/*
`resetSafe` resets DRV8874 when alarmed.
*/
void  DRV8874::resetSafe(int int_reset_time_ms = 1000, bool useDelay = true){
  //Return if not alarmed
  if (!checkAlarm()){
    return;
  }
  //Use delay for the reset
  if (useDelay){
    _resetSafeDelay(int_reset_time_ms);
    return;  
  }
  //No delay
  _resetSafeNoDelay(int_reset_time_ms);
}

/*
Resets the DRV8874 by driving the sleep pin using delay. 
*/
void _resetSafeDelay(int int_reset_time_ms){
  digitalWrite(_sleepPin, LOW);
  delay(reset_time_ms);
  digitalWrite(_sleepPin, HIGH);
  delay(MIN_RECOVER_TIME);
}
/*
Resets the DRV8874 by driving the sleep pin without using delay. 
*/
void _resetSafeNoDelay(int int_reset_time_ms){
  if (!_resetInProgress){
    digitalWrite(_sleepPin, LOW);
    _resetTime = millis();
    _resetInProgress = true;
    return;
  }
  //Wait X time before driving HIGH the sleep pin
  if ((millis()-_resetTime)>int_reset_time_ms){
    digitalWrite(_sleepPin, HIGH);
    _resetTime = millis();
  }
  //Allow time for driver to stabilize
  if ((millis()-_resetTime)>MIN_RECOVER_TIME){
    _resetInProgress = false;
  }
}

void  DRV8874::updateSpeed(float speed){
  if (_resetInProgress){
    return;
  }

  return;
}
void  DRV8874::rampSpeedAcc (float targetSpeed, float setAcc,      bool useLoop  = true){
  if (_resetInProgress){
    return;
  }
  return;
}
void  DRV8874::rampSpeedTime(float targetSpeed, float timeSeconds, bool useDelay = true){
  if (_resetInProgress){
    return;
  }
  return;
}
void  DRV8874::coastBrake(){
  return;
}

/*
`checkAlarm` returns true if DRV8874 is alarmed.
*/
bool  DRV8874::checkAlarm(){
  return !bool(digitalRead(_alarmPin));
}
float DRV8874::currentSpeed(){
  return;
}