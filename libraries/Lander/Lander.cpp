#include "Arduino.h"
#include "Lander.h"

landerDOF dof;
landerGPS gps;
landerServos servos;

void timeOut() {
	while (true) 
		delay(500);
}

void Lander::pointTo(Lander::latLon targetLoc) {
	//Lander::latLon currentLoc = gps.getCurrentLatLon();
	latLon currentLoc;
	currentLoc.north = 28.06;
	currentLoc.west = -82.42;
	//Currently set to JP Hall as a placeholder until GPS enabled

	float neededHeading = dof.compass.getNeededHeading(currentLoc, targetLoc);
	float currentHeading = dof.compass.getCurrentHeading();
	float panAngle = neededHeading - currentHeading;
	//panAngle ranges from -360 to 360, but our servo only ranges from 0 to 180
	if(panAngle < 0) {
		panAngle += 360.0;
	}
	//panAngle ranges from 0 to +360 now

	float currentAltitude = dof.altimeter.getCurrentAltitude();
	float tiltAngle = dof.altimeter.getNeededTiltAngle(currentLoc, targetLoc, currentAltitude);
	//tiltAngle ranges from 0 to 90 with 0 straight down; we need 90 to be straight down
	tiltAngle = 90.0 - tiltAngle;
	//now we can point at any spot from 0 to 180 pan, 0 to 90 tilt
	
	if (panAngle > 180)
	{
		panAngle = panAngle - 180;
		//pan in the opposite direction
		tiltAngle = 180 - tiltAngle;
		//tilt in the opposite direction
	}
	//now we have full 0 to 360 pan, 0 to 90 tilt.

	servos.setAngle("pan", panAngle);
	servos.setAngle("tilt", tiltAngle);
}

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