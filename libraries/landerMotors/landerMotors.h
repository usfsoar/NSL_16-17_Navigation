#include "Arduino.h"
#include <Servo.h>

class landerMotors {
	public:
		void leftOn(int val), rightOn(int val), leftOff(), rightOff();
		void init(), setLeftPin(int pin), setRightPin(int pin), setMinVal(int val);
		void enable(bool enable);
		bool isEnabled();
		int motorsEnabled, getMinVal();
		
	private:
		int leftMotorPin, rightMotorPin, minVal;
		Servo leftMotor, rightMotor;
};