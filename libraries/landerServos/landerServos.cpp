#include "Arduino.h"
#include "landerServos.h"
#include <avr/pgmspace.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// angles (deg), then matching PWM (note: set to within servo range to avoid burnout):
const PROGMEM int panRanges[4] = {0,180,480,200};
const PROGMEM int tiltRanges[4] = {-45,45,220,350};

const PROGMEM int tiltBegin = 468;

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
	if(servo == 1) { // pan
		int pulselen = constrainPWM(map(angle, panRanges[0], panRanges[1], panRanges[2], panRanges[3]), panRanges[2], panRanges[3]);
		pwm.setPWM(panServoPin, 0, pulselen);

	} else if(servo == 2) { // tilt
		int pulselen = constrainPWM(map(angle, tiltRanges[0], tiltRanges[1], tiltRanges[2], tiltRanges[3]), tiltRanges[2], tiltRanges[3]);
		pwm.setPWM(tiltServoPin, 0, pulselen);

	} 
}

void landerServos::setPin(int servo, int pin) {
	if(servo == 1) {
		panServoPin = pin;

	} else if(servo == 2) {
		tiltServoPin = pin;

	} 
}

void landerServos::goToIdle() {
	setAngle(1, 45);
	pwm.setPWM(tiltServoPin, 0, tiltBegin); // Outside normal range
}

bool landerServos::init() {
	pwm.begin();
	pwm.setPWMFreq(60);
	goToIdle();
	return true;
}
