#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class landerServos {
	public:
		void setAngle(int servo, int angle), setPin(int servo, int pin);
		void init(), enable(bool enable);
		bool isEnabled();
		int servosEnabled, constrainPWM(int val, int limitA, int limitB);
		
	private:
		int panServoPin, tiltServoPin;
};