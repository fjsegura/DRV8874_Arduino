/*
  DRV8874_Arduino.h - Library for using DRV8874 from Texas Instruments.
  Created by Francisco Segura, August 8, 2024.
  Released into the public domain.
*/
#ifndef DRV8874_Arduino_h
#define DRV8874_Arduino_h
#include "Arduino.h"
class DRV8874
{
  public:
    DRV8874(
	    int enIn1Pin,
	    int phIn2Pin,
	    int sleepPin,
	    int alarmPin,
	    bool invertControl = false,
	    bool enablePwmMode = false
	    );
    void  begin(bool pullupAlarm = false);
    void  resetSafe();
    void  updateSpeed(float speed);
    void  rampSpeedAcc (float targetSpeed, float setAcc,      bool useLoop  = true);
    void  rampSpeedTime(float targetSpeed, float timeSeconds, bool useDelay = true);
    void  coastBrake();
    bool  checkAlarm();
    float currentSpeed();
  private:
    int   _enIn1Pin;
    int   _phIn2Pin;
    int   _sleepPin;
    int   _alarmPin;
    bool  _alarmState;
    bool  _invertControl;
    bool  _enablePwmMode;
    float _speed;
    float _acceleration;

};
#endif
