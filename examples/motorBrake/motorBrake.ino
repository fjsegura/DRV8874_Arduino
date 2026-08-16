/*
  motorBrake.ino - Example demonstrating active braking with the DRV8874.

  The motor runs forward for 3 seconds, then brake() is called to engage
  slow-decay braking (actively resists rotation) rather than simply
  coasting to a stop. The sequence then repeats in reverse.

  Wiring (adjust pin numbers to match your board):
    IN1    -> pin 8
    IN2    -> pin 18  (must be PWM-capable for PWM mode)
    SLEEP  -> pin 17
    nFAULT -> pin 16  (pulled up internally via pullupAlarm = true below,
                       or externally with a pull-up resistor)

  The driver is created in PWM mode (enablePwmMode = true), which requires
  both IN1 and IN2 to be PWM-capable pins.
*/
#include <DRV8874_Arduino.h>

const int pin_IN1   = 8;
const int pin_IN2   = 18;
const int pin_SLEEP = 17;
const int pin_ALARM = 16;

// invertControl = false, enablePwmMode = true
DRV8874 motor(pin_IN1, pin_IN2, pin_SLEEP, pin_ALARM, false, true);

void setup() {
  Serial.begin(9600);

  // Pass true to use the MCU internal pull-up on nFAULT when no external
  // pull-up resistor is fitted on the board.
  motor.begin(/*pullupAlarm=*/true);
  motor.setDebug(true);

  // Clear any latched fault before starting.
  if (motor.checkAlarm()) {
    Serial.println("Fault detected on startup - clearing...");
    motor.clearFault();
  }

  // --- Forward run ---
  Serial.println("Running forward at 70%...");
  motor.setSpeed(70.0);
  delay(3000);

  // brake() engages slow-decay mode: the H-bridge actively resists rotation.
  // This stops the motor faster than setSpeed(0), which simply removes drive.
  Serial.println("Braking (slow-decay)...");
  motor.brake();
  delay(1500);

  // --- Reverse run ---
  Serial.println("Running reverse at 50%...");
  motor.setSpeed(-50.0);
  delay(3000);

  Serial.println("Braking again...");
  motor.brake();
  delay(1500);

  Serial.println("Sequence complete. Idling.");
}

void loop() {
  // Monitor for driver faults during idle and clear them without blocking.
  if (motor.checkAlarm()) {
    Serial.println("Fault detected - attempting non-blocking reset...");
    motor.clearFault(/*resetMs=*/1000, /*useDelay=*/false);
  }
  delay(100);
}
