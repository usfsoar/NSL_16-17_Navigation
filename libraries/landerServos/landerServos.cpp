#include "Arduino.h"
#include "landerServos.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define PANMIN  200
#define PANMAX  500
#define PANRANGE  118

#define TLTMIN  220
#define TLTMAX  300
#define TLTRANGE  45

int servosEnabled = -1;
int panServoPin, tiltServoPin;

void landerServos::setAngle(int servo, int val) {
  if(servo == 1) {
    int pulselen = map(val, 0, PANRANGE, PANMIN, PANMAX);
    pwm.setPWM(panServoPin, 0, pulselen);
    Serial.print(F("Pan servo angle set: "));
    Serial.println(val);

  } else if(servo == 2) {
    int pulselen = map(val, 0, TLTRANGE, TLTMIN, TLTMAX);
    pwm.setPWM(tiltServoPin, 0, pulselen);
    Serial.print(F("Tilt servo angle set: "));
    Serial.println(val);

  } else {
    Serial.println(F("Error at setAngle(servo, val) -- valid servos are 1 and 0."));
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
    Serial.println(F("Error at setPin(servo, pin) -- valid servos are 1 and 0."));
  }
}

void landerServos::enable(bool enable) {
	servosEnabled = (int)enable;
}

bool landerServos::isEnabled() {
	return servosEnabled;
}

void landerServos::init() {
  pwm.begin();
  pwm.setPWMFreq(60);
  yield();
  Serial.println(F("Servos intialized"));
}