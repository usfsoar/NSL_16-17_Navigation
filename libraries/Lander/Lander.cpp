#include "Arduino.h"
#include "Lander.h"

landerDOF dof;
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
}

bool Lander::init() {
	if (dof.dofEnabled == -1 || dof.compass.compassEnabled == -1 || dof.altimeter.altimeterEnabled == -1 || motors.motorsEnabled == -1 || gps.gpsEnabled == -1) { /*FIXME*/
		Serial.println("All sensors must be enabled or disabled.");
		timeOut();
	}
	
	if (dof.isEnabled())
		dof.init();
	if (motors.isEnabled())
		motors.init();
	if (gps.isEnabled())
		gps.init();
	
	Serial.println("Successfully initialized");
}








