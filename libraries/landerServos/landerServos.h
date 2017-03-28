#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class landerServos {
	public:
		void setAngle(int servo, int val), setPin(int servo, int pin);
		void init(), enable(bool enable);
		bool isEnabled();
		int servosEnabled;
		
	private:
		int panServoPin, tiltServoPin;
};