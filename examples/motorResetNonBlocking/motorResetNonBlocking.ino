#include <DRV8874_Arduino.h>

// Demonstrates non-blocking fault clearing: clearFault(resetMs, useDelay=false)
// must be called repeatedly; it never blocks the loop. While the reset is in
// progress the library ignores setSpeed() calls (safe default).

int pin_IN1 = 8;
int pin_IN2 = 18;
int pin_SLEEP = 17;
int pin_ALARM = 16;
bool pullupAlarm = true;

DRV8874 motor = DRV8874(pin_IN1, pin_IN2, pin_SLEEP, pin_ALARM);

unsigned long lastToggle = 0;
bool running = false;
const unsigned long TOGGLE_INTERVAL_MS = 5000;
const int RESET_TIME_MS = 250;

void setup() {
  Serial.begin(9600);
  motor.begin(pullupAlarm);
  // Wait for the first byte from the serial monitor before proceeding,
  // so output from setup() is not lost on boards that re-enumerate USB.
  while (!Serial.available()) {
    // Servicing clearFault here keeps a latched startup fault from blocking.
    motor.clearFault(RESET_TIME_MS, false);
    delay(10);
  }
  Serial.println("Ready. Motor will alternate forward/stop every 5 s.");
}

void loop() {
  // Always service fault recovery without blocking.
  motor.clearFault(RESET_TIME_MS, false);

  if (motor.checkAlarm()) {
    Serial.println("Motor alarmed; waiting for recovery...");
    return;
  }

  if (millis() - lastToggle >= TOGGLE_INTERVAL_MS) {
    lastToggle = millis();
    running = !running;
    motor.setSpeed(running ? 90.0f : 0.0f);
    Serial.println(running ? "Forward" : "Stop");
  }
}
