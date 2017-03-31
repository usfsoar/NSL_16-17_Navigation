#include "Arduino.h"
#include "Lander.h"

landerDOF dof;
landerGPS gps;
landerServos servos;

void Lander::timeOut() {
	while (true) 
		delay(500);
}

void Lander::pointTo(float targetLoc[2]) {
	//Lander::latLon currentLoc = gps.getCurrentLatLon();

	float currentLoc[2] = {28.054506, -82.425972};
	//Currently set to JP Hall as a placeholder until GPS enabled

	int * orientation = dof.ahrs.getCurrentOrientation();

	int neededHeading = dof.ahrs.getNeededHeading(currentLoc, targetLoc);
	Serial.print(F("Needed heading: ")); 
	Serial.println(neededHeading);
	int currentHeading = orientation[0];
	Serial.print(F("Current heading: ")); 
	Serial.println(currentHeading);

	int panAngle = neededHeading - currentHeading;
	if(panAngle > 180) {
		panAngle -= 360;
	} else if(panAngle < -180) {
		panAngle += 360;
	}
	Serial.print(F("UnCompensated Pan Angle: ")); 
	Serial.println(panAngle);
	//panAngle ranges from -180 (right) to +180 (left) now (because normal angles go CCW)

	float altitude = dof.altimeter.getCurrentAltitude();

	int * angles = dof.ahrs.getCompensatedAngles(orientation, altitude, panAngle, currentLoc, targetLoc);

	panAngle = angles[0];
	int tiltAngle = angles[1];

	Serial.print(F("Compensated Pan Angle: ")); 
	Serial.println(panAngle);
	Serial.print(F("Compensated Tilt Angle: ")); 
	Serial.println(tiltAngle);
	
	//int tiltAngle = dof.altimeter.getNeededTiltAngle(currentLoc, targetLoc, currentAltitude);
	//tiltAngle ranges from 0 (down) to 90 (horizontal)
	
	//servos can only go 180deg, not 360
	if (panAngle > 0) { //looking to the left
		panAngle = 180 - panAngle;
		//pan in the opposite direction
		tiltAngle = -tiltAngle;
		//tilt in the opposite direction
	} else { //looking to the right
		panAngle = -panAngle;
	}

	Serial.print(F("Pan servo setting: ")); 
	Serial.println(panAngle);
	Serial.print(F("Tilt servo setting: ")); 
	Serial.println(tiltAngle);

	servos.setAngle(1, panAngle);
	servos.setAngle(2, tiltAngle);
}

bool Lander::init() {
	if (dof.dofEnabled == -1 || servos.servosEnabled == -1 || gps.gpsEnabled == -1) {
		Serial.println(F("All sensors and components must be enabled or disabled."));
		timeOut();
	}
	
	if (dof.isEnabled())
		dof.init();
	if (servos.isEnabled())
		servos.init();
	if (gps.isEnabled())
		gps.init();
	
	Serial.println(F("Lander initialized"));
}