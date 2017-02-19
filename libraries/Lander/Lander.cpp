#include "Arduino.h"
#include "Lander.h"

landerCompass compass;
landerGPS gps;
landerMotors motors;

void timeOut() {
	while (true) 
		delay(500);
}

void Lander::abort() {
	motors.leftOff();
	motors.rightOff();
}

void Lander::navigateTo(int reductionFactor, int heading){
	int start = motors.getMinVal();
	int difference = compass.getCurrentHeading() - heading;
 
	if (difference < 0) {
		difference += 360;
	}

	if (difference <= 180) {
		motors.rightOn(start + (difference / reductionFactor));
		motors.leftOn(start);
		} else {
			motors.leftOn(start + ((360 - difference) / reductionFactor));
			motors.rightOn(start);
		}  
	
	}

bool Lander::init() {
	if (compass.compassEnabled == -1 || motors.motorsEnabled == -1 || gps.gpsEnabled == -1) {
		Serial.println("All sensors must be enabled or disabled..");
		timeOut();
	}
	
	if (compass.isEnabled())
		compass.init();
	if (motors.isEnabled())
		motors.init();
	if (gps.isEnabled())
		gps.init();
	
	Serial.println("Successfully initialized");
}








