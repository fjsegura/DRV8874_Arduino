#include <DRV8874_Arduino.h>

int pin_IN1 = 8;
int pin_IN2 = 18;
int pin_SLEEP = 17;
int pin_ALARM = 16;
bool pullupAlarm = true;

DRV8874 motor = DRV8874(pin_IN1, pin_IN2, pin_SLEEP, pin_ALARM);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial.available()){
    // Wait for the first byte from the serial monitor before proceeding,
    // so output from setup() is not lost on boards that re-enumerate USB.
    delay(10);
  }
  motor.begin(pullupAlarm);
  motor.clearFault();
}

void loop() {
  // put your main code here, to run repeatedly:
  motor.setSpeed(90.0);
  delay(10000);
  motor.setSpeed(0);
  delay(10000);
}
