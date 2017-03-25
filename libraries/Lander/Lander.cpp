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

	float currentLoc[2] = {28.069335, -82.401810};
	//Currently set to JP Hall as a placeholder until GPS enabled

	int neededHeading = dof.compass.getNeededHeading(currentLoc, targetLoc);
	Serial.print(F("Needed heading.: ")); 
	Serial.println(neededHeading);
	int currentHeading = dof.compass.getCurrentHeading();
	Serial.print(F("Current heading: ")); 
	Serial.println(currentHeading);
	int panAngle = neededHeading - currentHeading;
	//panAngle ranges from -360 to 360, but our servo only ranges from 0 to 180
	if(panAngle < 0) {
		panAngle += 360.0;
	}
	//panAngle ranges from 0 to +360 now

	float currentAltitude = dof.altimeter.getCurrentAltitude();
	int tiltAngle = dof.altimeter.getNeededTiltAngle(currentLoc, targetLoc, currentAltitude);
	//now we can point at any spot from 0 to 180 pan, 0 to 45 tilt
	
	if (panAngle > 180)
	{
		panAngle = panAngle - 180;
		//pan in the opposite direction
		tiltAngle = -tiltAngle;
		//tilt in the opposite direction
	}
	//now we have full 0 to 360 pan, 0 to 45 tilt.
	Serial.print(F("Pan servo setting to... ")); 
	Serial.println(panAngle);
	Serial.print(F("Tilt servo setting to... ")); 
	Serial.println(tiltAngle);


	servos.setAngle(1, panAngle);
	servos.setAngle(2, tiltAngle);
	Serial.print(F("Pan servo set to: ")); 
	Serial.println(panAngle);
	Serial.print(F("Tilt servo set to: ")); 
	Serial.println(tiltAngle);

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