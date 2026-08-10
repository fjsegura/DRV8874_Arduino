# DRV8874 Arduino library


This is a library for the Texas Instruments DRV8874 motor driver. Heavily inspired in the [Adafruit Arduino Libraries](https://github.com/adafruit)

This is a work in progress, please use with caution.

## Quick start

```cpp
#include <DRV8874_Arduino.h>

DRV8874 motor(pinIn1, pinIn2, pinSleep, pinAlarm);

void setup() {
  motor.begin(true);      // true = enable internal pull-up on alarm pin
  motor.resetIfAlarm();   // reset driver if alarm is active
}

void loop() {
  motor.setSpeed(75.0);   // speed from -100.0 to 100.0
  delay(1000);
  motor.setSpeed(0.0);
  delay(1000);
}
```

## Main API

- `setSpeed(percent)` / `setSpeedPercent(percent)`: set motor speed from `-100.0` to `100.0`.
- `brake()`: apply brake mode.
- `isAlarmed()`: read alarm status.
- `resetIfAlarm(resetTimeMs, blocking)`: clear driver fault using sleep pin reset.
- `getSpeed()`: returns the last applied capped speed.

Legacy methods (`updatePossibleSpeed`, `resetSafe`, `checkAlarm`, `currentSpeed`) are still available for compatibility.
  
Written by Francisco Segura. Pura vida :)
MIT license, all text above must be included in any redistribution.
