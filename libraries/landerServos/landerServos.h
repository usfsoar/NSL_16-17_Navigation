#include "Arduino.h"
#include <Servo.h>

class landerServos {
	public:
		void setAngle(int servo, int val), setPin(int servo, int pin);
		void init(), enable(bool enable);
		bool isEnabled();
		int getAngle(int servo);
		
	private:
		int panServoPin, tilServoPin;
		Servo panServo, tiltServo;
};