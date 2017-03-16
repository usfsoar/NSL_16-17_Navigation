#include "Arduino.h"
#include "landerServos.h"

int servosEnabled = -1;
int panServoPin, tiltServoPin;
Servo panServo;
Servo tiltServo;

void landerServos::setAngle(string servo, int val) {
  if(servo == "pan" || servo == "panServo") {
    panServo.write(val);
  } else if(servo == "tilt" || servo == "tiltServo") {
    tiltServo.write(val);
  } else {
    Serial.println("Error with setAngle(servo, val) -- invalid servo name.");
  }
}

int landerServos::getAngle(string servo) {
  if(servo == "pan" || servo == "panServo") {

    if (panServo.attached()) {
      return panServo.read();
    } else {
      return -1;
    }
    
  } else if(servo == "tilt" || servo == "tiltServo") {

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

void landerServos::setPin(string servo, int pin) {
  if(servo == "pan" || servo == "panServo") {
    panServoPin = pin;
  } else if(servo == "tilt" || servo == "tiltServo") {
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