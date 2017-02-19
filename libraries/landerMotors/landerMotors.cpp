#include "Arduino.h"
#include "landerMotors.h"


int leftMotorPin, rightMotorPin, minMotorVal;
int motorsEnabled = -1;
Servo leftMotor;
Servo rightMotor;

void landerMotors::leftOn(int val) {
  leftMotor.writeMicroseconds(val);
}

void landerMotors::rightOn(int val) {
  rightMotor.writeMicroseconds(val);
}

void landerMotors::leftOff() {
  leftMotor.writeMicroseconds(1100);
}

void landerMotors::rightOff() {
  rightMotor.writeMicroseconds(1100);
}

void landerMotors::setLeftPin(int pin) {
	leftMotorPin = pin;
}

void landerMotors::setRightPin(int pin) {
	rightMotorPin = pin;
}

void landerMotors::setMinVal(int val) {
	minVal = val;
}

int landerMotors::getMinVal() {
	return minVal;
}

void landerMotors::enable(bool enable) {
	motorsEnabled = (int)enable;
}

bool landerMotors::isEnabled() {
	return motorsEnabled;
}

void landerMotors::init() {
  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);
  rightOff();
  leftOff();
  delay(10000);  
}