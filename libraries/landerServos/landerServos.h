#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class landerServos {
	public:
		void setAngle(int servo, int angle), setPin(int servo, int pin);
		bool init();
		int constrainPWM(int val, int limitA, int limitB);
		
	private:
		int panServoPin, tiltServoPin;
};