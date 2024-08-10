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
		    bool invertControl /*= false*/,
		    bool enablePwmMode /*= false*/
		    )
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

void DRV8874::begin(bool pullupAlarm /*= true*/)
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
  analogWriteResolution(_enIn1Pin, _PWM_RESOLUTION_DRIVER);
  analogWriteResolution(_phIn2Pin, _PWM_RESOLUTION_DRIVER);
  _maxPwmValue = int(pow(2.0, _PWM_RESOLUTION_DRIVER)) - 1;
}

/*
`checkAlarm` returns true if DRV8874 is alarmed.
*/
bool  DRV8874::checkAlarm(){
  return !bool(digitalRead(_alarmPin));
}

/*
`resetSafe` resets DRV8874 when alarmed.
*/
void  DRV8874::resetSafe(int int_reset_time_ms /*= 1000*/, bool useDelay /*= true*/){
  //Return if not alarmed
  if (!DRV8874::checkAlarm() && !_resetInProgress){
    return;
  }
  //Use delay for the reset
  if (useDelay){
    DRV8874::_resetSafeDelay(int_reset_time_ms);
    return;  
  }
  //No delay
  DRV8874::_resetSafeNoDelay(int_reset_time_ms);
}

/*
Resets the DRV8874 by driving the sleep pin using delay. 
*/
void DRV8874::_resetSafeDelay(int int_reset_time_ms){
  digitalWrite(_sleepPin, LOW);
  delay(int_reset_time_ms);
  digitalWrite(_sleepPin, HIGH);
  delay(_MIN_RECOVER_TIME);
}
/*
Resets the DRV8874 by driving the sleep pin without using delay. 
*/
void DRV8874::_resetSafeNoDelay(int int_reset_time_ms){
  if (!_resetInProgress){
    digitalWrite(_sleepPin, LOW);
    _resetTime = millis();
    _resetInProgress = true;
    _waitInProgress  = false;
    return;
  }
  //Wait X time before driving HIGH the sleep pin
  if (((millis()-_resetTime)>int_reset_time_ms) && !_waitInProgress){
    digitalWrite(_sleepPin, HIGH);
    _waitInProgress = true;
    _resetTime = millis();
  }
  //Allow time for driver to stabilize
  if ((millis()-_resetTime)>_MIN_RECOVER_TIME){
    _resetInProgress = false;
    _waitInProgress = false;
  }
}
/*
Updates the motor speed by a `float speed` value. Value has to be between -100.0 and 100.0
Any   value higher or lower will be capped.
*/
void  DRV8874::updatePossibleSpeed(float speed){
  //Don't update speed if the reset is in progress
  if (_resetInProgress){
    return;
  }
  //Don't update speed if acc is in progress
  if (_accInProgress){
    return;
  }
  _updateSpeed(speed);
}

/*
Updates the motor speed by a `float speed` value. Value has to be between -100.0 and 100.0
Any   value higher or lower will be capped.
*/
void  DRV8874::_updateSpeed(float speed){
  _speed = speed;
  float cappedSpeed = DRV8874::_capSpeed(_speed);
  if (_enablePwmMode){
    DRV8874::_updateSpeedPwm(cappedSpeed);
    return;
  }
  DRV8874::_updateSpeedPhEn(cappedSpeed);
}

/*
Updates the motor speed by a `float speed` value. Using the Phase / Enable mode.    
*/
void DRV8874::_updateSpeedPhEn(float speed){
  if (speed == 0){
    digitalWrite(_enIn1Pin, LOW);
    digitalWrite(_phIn2Pin, LOW);
    return;
  } 
  //Set direction
  if (speed > 0){
    //Postive value
    digitalWrite(_phIn2Pin, !_invertControl);
  } else {
    //Negative value
    digitalWrite(_phIn2Pin, _invertControl);
  }
  //Set speed
  float absSpeed = abs(speed);
  analogWrite(_enIn1Pin, DRV8874::_pwmValue(absSpeed));
}

/*
Updates the motor speed by a `float speed` value. Using the PWM mode
*/
void DRV8874::_updateSpeedPwm(float speed){
  if (speed == 0){
    digitalWrite(_enIn1Pin, LOW);
    digitalWrite(_phIn2Pin, LOW);
    return;
  } 
  int pwmValue1 = 0;
  int pwmValue2 = 0;

  //Set speed
  float absSpeed = abs(speed);
  int speedPwm = _pwmValue(absSpeed);
  //Set direction
  if (speed > 0){
    //Postive value
    pwmValue1 = speedPwm;
    pwmValue2 = 0;
  } else {
    //Negative value
    pwmValue1 = 0;
    pwmValue2 = speedPwm;
  }
  //Invert direction
  if (_invertControl){
    int tempPwm = pwmValue1;
    pwmValue1 = pwmValue2;
    pwmValue2 = tempPwm;
  } 
  analogWrite(_enIn1Pin, pwmValue1);
  analogWrite(_phIn2Pin, pwmValue2);
}

int DRV8874::_pwmValue(float absSpeed) {
  //Reduce float
  int absSpeedInt = int(absSpeed*100.0); 
  int pwmValue = map(absSpeed,0, 10000, 0, _maxPwmValue);
  return pwmValue;
}
float DRV8874::_capSpeed(float speed){
  if (speed > 100.0){
    return 100.0;
  } 
  if (speed < -100.0){
    return -100.0;
  }
  return speed;
}

void  DRV8874::rampSpeedAcc (float targetSpeed, float setAcc,      bool useLoop  /*= true*/){
  if (_resetInProgress){
    return;
  }
  return;
}
void  DRV8874::rampSpeedTime(float targetSpeed, float timeSeconds, bool useDelay /*= true*/){
  if (_resetInProgress){
    return;
  }
  return;
}
/*
This function brakes the motors by driving the H-bridge in slow decay mode.
*/
void  DRV8874::brake(){
  if (_resetInProgress){
    return;
  }
  _accInProgress = false;
  if (_enablePwmMode){
    _brakePwm();
    return;
  }
  _brakePhEn();
}
/*
Brake setting both driver input HIGH.
*/
void  DRV8874::_brakePwm(){
  digitalWrite(_enIn1Pin, HIGH);
  digitalWrite(_phIn2Pin, HIGH);
}
/*
Brake setting enable LOW.
*/
void  DRV8874::_brakePhEn(){
  digitalWrite(_enIn1Pin, LOW);
}
/*
Return current `float speed` value.
*/
float DRV8874::currentSpeed(){
  return _speed;
}