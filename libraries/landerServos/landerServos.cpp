#include "Arduino.h"
#include "landerServos.h"
#include <avr/pgmspace.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// angles (deg), then matching PWM (note: set to within servo range to avoid burnout):
const PROGMEM int panRanges[4] = {0,180,485,200};
const PROGMEM int tiltRanges[4] = {-45,45,220,350};

int panServoPin = 0, tiltServoPin = 3;

int landerServos::constrainPWM(int val, int limitA, int limitB) {
  if(val > limitA && val > limitB) {
    return max(limitA, limitB);
  } else if(val < limitA && val < limitB) {
    return min(limitA, limitB);
  } else {
    return val;
  }
}

void landerServos::setAngle(int servo, int angle) {
  if(servo == 1) {
    int pulselen = constrainPWM(map(angle, panRanges[0], panRanges[1], panRanges[2], panRanges[3]), panRanges[2], panRanges[3]);
    pwm.setPWM(panServoPin, 0, pulselen);
    Serial.print(F("Pan servo angle set: "));
    Serial.print(angle);
    Serial.print(F(" deg; "));
    Serial.println(pulselen);

  } else if(servo == 2) {
    int pulselen = constrainPWM(map(angle, tiltRanges[0], tiltRanges[1], tiltRanges[2], tiltRanges[3]), tiltRanges[2], tiltRanges[3]);
    pwm.setPWM(tiltServoPin, 0, pulselen);
    Serial.print(F("Tilt servo angle set: "));
    Serial.print(angle);
    Serial.print(F(" deg; "));
    Serial.println(pulselen);

  } else {
    Serial.println(F("Error at setAngle(servo, val) -- valid servos are 1 (pan) and 2 (tilt)."));
  }
}

void landerServos::setPin(int servo, int pin) {
  if(servo == 1) {
    panServoPin = pin;
    Serial.print(F("Pan pin set:"));
    Serial.println(pin);

  } else if(servo == 2) {
    tiltServoPin = pin;
    Serial.print(F("Tilt pin set:"));
    Serial.println(pin);

  } else {
    Serial.println(F("Error at setPin(servo, pin) -- valid servos are 1 (pan) and 2 (tilt)."));
  }
}

bool landerServos::init() {
  pwm.begin();
  pwm.setPWMFreq(60);
  Serial.println(F("Servos intialized"));
  return true;
}
