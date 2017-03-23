#include "Arduino.h"
#include "landerServos.h"

int servosEnabled = -1;
int panServoPin, tiltServoPin;
Servo panServo;
Servo tiltServo;

void landerServos::setAngle(int servo, int val) {
  if(servo == 1) {
    panServo.write(val);
  } else if(servo == 2) {
    tiltServo.write(val);
  } else {
    Serial.println("Error with setAngle(servo, val) -- invalid servo name.");
  }
}

int landerServos::getAngle(int servo) {
  if(servo == 1) {

    if (panServo.attached()) {
      return panServo.read();
    } else {
      return -1;
    }
    
  } else if(servo == 2) {

    if (tiltServo.attached()) {
      return tiltServo.read();
    } else {
      return -1;
    }

  } else {
    Serial.println("Error with readAngle(servo) -- invalid servo name.");
    return -2;
  }
}

void landerServos::setPin(int servo, int pin) {
  if(servo == 1) {
    panServoPin = pin;
  } else if(servo == 2) {
    tiltServoPin = pin;
  } else {
    Serial.println("Error with readAngle(servo) -- invalid servo name.");
  }
}

void landerServos::enable(bool enable) {
	servosEnabled = (int)enable;
}

bool landerServos::isEnabled() {
	return servosEnabled;
}

void landerServos::init(int panServoPin, int tiltServoPin) {
  //pinMode(1,OUTPUT)
  panServo.attach(panServoPin);
  tiltServo.attach(tiltServoPin);
  if(panServo.attached() && tiltServo.attached()) {
    Serial.println("Servos intitialized.");
  }
}