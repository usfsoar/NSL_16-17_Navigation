#include "Arduino.h"
#include "Lander.h"

landerDOF dof;
landerGPS gps;
//landerMotors motors;
landerServos servos;

void timeOut() {
	while (true) 
		delay(500);
}

/*void Lander::abort() {
	motors.leftOff();
	motors.rightOff();
}*/

/*void Lander::navigateTo(int reductionFactor, int heading){
	int start = motors.getMinVal();
	int difference = dof.compass.getCurrentHeading() - heading;
 
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
}*/

bool Lander::init() {
	if (dof.dofEnabled == -1 || servos.servosEnabled == -1 || gps.gpsEnabled == -1) {
		Serial.println("All sensors and components must be enabled or disabled.");
		timeOut();
	}
	
	if (dof.isEnabled())
		dof.init();
	if (servos.isEnabled())
		servos.init();
	if (gps.isEnabled())
		gps.init();
	
	Serial.println("Successfully initialized");
}








