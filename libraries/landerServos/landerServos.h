#include "Arduino.h"
#include <Servo.h>

class landerServos {
	public:
		void setAngle(string servo, int val), setPin(string servo, int pin);
		void init(), enable(bool enable);
		bool isEnabled();
		int getAngle(string servo);
		
	private:
		int panServoPin, tilServoPin;
		Servo panServo, tiltServo;
};