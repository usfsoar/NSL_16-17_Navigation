#include "Arduino.h"
#include "Lander.h"

landerDOF dof;
landerGPS gps;
landerServos servos;

// Helper Functions:

float Lander::degToRad(int deg) {
	return (deg * PI / 180);
}

float Lander::degToRadFloat(float deg) {
	return (deg * PI / 180);
}

int Lander::radToDeg(float rad) {
	return (int)float(rad * 180 / PI);
}

// Latitude and Longitude Operations:

int Lander::getNeededHeading(float currLoc[2], float neededLoc[2]) {
	float deltaLon = neededLoc[1] - currLoc[1];

	// Forward azimuth formula:
	float yval = sin(deltaLon)*cos(neededLoc[0]),
			xval = cos(currLoc[0])*sin(neededLoc[0]) - sin(currLoc[0])*cos(neededLoc[0])*cos(deltaLon);
	float radAngle = atan2(yval,xval);

	int angle = radToDeg(radAngle);

	Serial.print(F("Needed heading: "));
	Serial.println(angle);
	return angle;
}

float Lander::getDistanceBetween(float locA[2], float locB[2]) {
	float radius = 6371000.0; // Radius of Earth, km
	float radLocA[2], radLocB[2], dLoc[2];

	// Radians (deg needs to be floats, because it's lat and lon):
	radLocA[0] = degToRadFloat(locA[0]); 
	radLocA[1] = degToRadFloat(locA[1]);
	radLocB[0] = degToRadFloat(locB[0]);
	radLocB[1] = degToRadFloat(locB[1]);

	dLoc[0] = sin((radLocB[0] - radLocA[0])/2);
	dLoc[1] = sin((radLocB[1] - radLocA[1])/2);

	// Haversine Formula:
	float dist = 2.0 * radius * asin(sqrt(dLoc[0] * dLoc[0] + cos(radLocA[0]) * cos(radLocB[0]) * dLoc[1] * dLoc[1]));

	Serial.print(F("Distance calculated: "));
	Serial.println(dist);
	setCommDistance(dist);
	return dist; // Meters
}

// Gyroscopic Compensation:

int * Lander::getCompensatedAngles(int hpr[3], float alt, float currentLoc[2], float targetLoc[2]) {
	static int compAngles[2];

	int neededHeading = getNeededHeading(currentLoc, targetLoc);

	// Uncompensated pan angle, so we don't have to compensate for yaw later.
	int panAngle = neededHeading - hpr[0];
	if(panAngle > 180) {
		panAngle -= 360;
	} else if(panAngle < -180) {
		panAngle += 360;
	}

	Serial.print(F("UnCompensated Pan Angle: ")); 
	Serial.println(panAngle);

	float pan = degToRad(panAngle), 
		dist = getDistanceBetween(currentLoc, targetLoc), 
		pitch = degToRad(hpr[1]), 
		roll = degToRad(hpr[2]);

	// Magic (change in basis using pitch and roll):
	float compVector[3] = {
		dist*cos(pan)*cos(pitch) + alt*sin(pitch),
		dist*sin(pan)*cos(roll) + dist*cos(pan)*sin(pitch)*sin(roll) - alt*cos(pitch)*sin(roll),
		-dist*sin(pan)*sin(roll) + dist*cos(pan)*sin(pitch)*cos(roll) - alt*cos(pitch)*cos(roll)
	};

	float panPrime = atan2(compVector[1],compVector[0]);
	float distPrime = compVector[1]/sin(panPrime);
	if (abs(distPrime - compVector[0]/cos(panPrime)) > 0.1) {
		Serial.println(F("WARNING: Error with compensated angles function."));
	}
	float tiltPrime = atan2(distPrime,(-compVector[2]));

	compAngles[0] = radToDeg(panPrime);
	compAngles[1] = radToDeg(tiltPrime);

	return compAngles;
}

// Actual camera aiming

void Lander::pointTo(float targetLoc[2]) {
	//Lander::latLon currentLoc = gps.getCurrentLatLon();

	float currentLoc[2] = {28.054506, -82.425972};
	//Currently set to JP Hall as a placeholder until GPS enabled

	int * orientation = dof.getCurrentOrientation();
	float altitude = dof.getCurrentAltitude();

	if(orientation[0] != 0 && altitude > 3) {
		int * angles = getCompensatedAngles(orientation, altitude, currentLoc, targetLoc);
		int panAngle = angles[0], tiltAngle = angles[1];

		Serial.print(F("Compensated Pan Angle: ")); 
		Serial.println(panAngle);
		Serial.print(F("Compensated Tilt Angle: ")); 
		Serial.println(tiltAngle);
		
		//servos can only go 180deg, not 360
		if (panAngle > 0) { //looking to the left
			panAngle = 180 - panAngle;
			tiltAngle = -tiltAngle;
		} else { //looking to the right
			panAngle = -panAngle;
		}

		Serial.print(F("Pan servo setting: ")); 
		Serial.println(panAngle);
		Serial.print(F("Tilt servo setting: ")); 
		Serial.println(tiltAngle);

		servos.setAngle(1, panAngle);
		servos.setAngle(2, tiltAngle);
	} else {
		Serial.println(F("WARNING: Altitude or heading returned strange value (likely error). Retrying."));
	}
}

void Lander::errorCheck() {
	//setDofError(dof.hasError()); TODO Uncomment
	setgpsHasFix(gps.hasFix());
	setDofError(false);
}

// Initiation

bool Lander::init() {
	dof.init();
	gps.init();
	servos.init();
	
	return true;
}