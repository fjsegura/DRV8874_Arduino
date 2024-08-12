/*
  DRV8874_Arduino.h - Library for using DRV8874 from Texas Instruments.
  Created by Francisco Segura, August 8, 2024.
  Released into the public domain.
*/
#ifndef DRV8874_Arduino_h
#define DRV8874_Arduino_h
#include "Arduino.h"

//Used for the reset safe with delay, minimun time before continuing
#define _MIN_RECOVER_TIME 10

//Override PWM resolution TODO: FIX THIS OVERRIDE
#define ENABLE_PWM_RESOLUTION_OVERRIDE 0
//Set the resolution for the PWM driver, should match MCU pwm resolution
#define _PWM_RESOLUTION_DRIVER 8
//Set const to 1 to enable serial debug 
#define DRV887X_DEBUG_SERIAL 0


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
    void  resetSafe(int int_reset_time_ms = 1000, bool useDelay = true);
    void  updatePossibleSpeed(float speed);
    void  rampSpeedAcc (float targetSpeed, float setAcc,      bool useLoop  = true);
    void  rampSpeedTime(float targetSpeed, float timeSeconds, bool useDelay = true);
    void  brake();
    bool  checkAlarm();
    float currentSpeed();
  private:
    int   _enIn1Pin;
    int   _phIn2Pin;
    int   _sleepPin;
    int   _alarmPin;
    int   _maxPwmValue;
    bool  _alarmState;
    bool  _invertControl;
    bool  _enablePwmMode;
    bool  _resetInProgress;
    bool  _waitInProgress;
    bool  _accInProgress;
    long  _resetTime;
    float _speed;
    float _acceleration;
    void  _updateSpeed(float speed);
    void  _resetSafeDelay(int int_reset_time_ms);
    void  _resetSafeNoDelay(int int_reset_time_ms);
    void  _updateSpeedPwm(float speed);
    void  _updateSpeedPhEn(float speed);
    float _capSpeed(float speed);
    int   _pwmValue(float absSpeed);
    void  _brakePwm();
    void  _brakePhEn();
    void  _debugSerial(String msg);
};
#endif
