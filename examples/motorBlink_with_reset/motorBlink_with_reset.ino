#include <DRV8874_Arduino.h>

#define PIN_IN1 0
#define PIN_IN2 0
#define PIN_SLEEP 0
#define PIN_ALARM 0

DRV8874 motor = DRV8874(PIN_IN1,PIN_IN2,PIN_SLEEP,PIN_ALARM);

void setup() {
  // put your setup code here, to run once:
  motor.begin();
  motor.resetSafe();
}

void loop() {
  // put your main code here, to run repeatedly:
  motor.updatePossibleSpeed(90.0);
  delay(1000);
  motor.updatePossibleSpeed(0);
  delay(1000);
}
