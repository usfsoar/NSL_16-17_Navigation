#include "Arduino.h"
#include "dofAHRS.h"

int ahrsEnabled = -1;

Adafruit_LSM303_Accel_Unified ahrsaccel(30301);
Adafruit_LSM303_Mag_Unified   ahrsmag(30302);
Adafruit_BMP085_Unified       ahrsbmp(18001);

Adafruit_Simple_AHRS sahrs(&ahrsaccel, &ahrsmag);

float dofAHRS::degToRad(int deg) {
  return (deg * PI / 180);
}

float dofAHRS::degToRadFloat(float deg) {
  return (deg * PI / 180);
}

int dofAHRS::radToDeg(float rad) {
  return (int)float(rad * 180 / PI);
}

int * dofAHRS::getCurrentOrientation() {
	static int hpr[3];

	sensors_vec_t orientation;

	if (sahrs.getOrientation(&orientation)) {
	/* 'orientation' should have valid .roll and .pitch fields */
		Serial.print(F("Orientation: "));
		Serial.print(orientation.heading);
		Serial.print(F(" "));
		Serial.print(orientation.pitch);
		Serial.print(F(" "));
		Serial.println(orientation.roll);
	} else {
		Serial.print(F("ERROR: Did not recieve AHRS event."));
	}

	hpr[0] = (int)round(orientation.heading);
	hpr[1] = (int)round(orientation.pitch);
	hpr[2] = (int)round(orientation.roll);

	return hpr;
}

float dofAHRS::getDistanceBetween(float locA[2], float locB[2]) {
  float radius = 6371000.0; // Radius of Earth, km
  float radLocA[2], radLocB[2], dLoc[2];

  // Radians (needs to be floats, because of the huge radius):
  radLocA[0] = degToRadFloat(locA[0]); 
  radLocA[1] = degToRadFloat(locA[1]);
  radLocB[0] = degToRadFloat(locB[0]);
  radLocB[1] = degToRadFloat(locB[1]);

  dLoc[0] = sin((radLocB[0] - radLocA[0])/2);
  dLoc[1] = sin((radLocB[1] - radLocA[1])/2);

  // Haversine (from http://stackoverflow.com/questions/10198985/calculating-the-distance-between-2-latitudes-and-longitudes-that-are-saved-in-a):
  float dist = 2.0 * radius * asin(sqrt(dLoc[0] * dLoc[0] + cos(radLocA[0]) * cos(radLocB[0]) * dLoc[1] * dLoc[1]));

  Serial.print(F("Distance calculated: "));
  Serial.println(dist);
  return dist; // Meters
}

int * dofAHRS::getCompensatedAngles(int hpr[3], float alt, int panAngle, float currentLoc[2], float targetLoc[2]) {
	static int compAngles[2];
	float pan = degToRad(panAngle), 
		dist = getDistanceBetween(currentLoc, targetLoc), 
		pitch = degToRad(hpr[1]), 
		roll = degToRad(hpr[2]);

	// Magic:
	float compVector[3] = {
		dist*cos(pan)*cos(pitch) + alt*sin(pitch),
		dist*sin(pan)*cos(roll) + dist*cos(pan)*sin(pitch)*sin(roll) - alt*cos(pitch)*sin(roll),
		-dist*sin(pan)*sin(roll) + dist*cos(pan)*sin(pitch)*cos(roll) - alt*cos(pitch)*cos(roll)
	};

	float panPrime = atan2(compVector[1],compVector[0]);
	float distPrime = compVector[1]/sin(panPrime);
	if (abs(distPrime - compVector[0]/cos(panPrime)) > 0.1) {
		Serial.println(F("ERROR!"));
	}
	float tiltPrime = atan2(distPrime,(-compVector[2]));

	compAngles[0] = radToDeg(panPrime);
	compAngles[1] = radToDeg(tiltPrime);

	return compAngles;
}

int dofAHRS::getNeededHeading(float currLoc[2], float neededLoc[2]) {
	float deltaLon = neededLoc[1] - currLoc[1]; //longitude
	float y = sin(deltaLon) * cos(neededLoc[0]);
	float x = cos(currLoc[0])*sin(neededLoc[0]) - sin(currLoc[0])*cos(neededLoc[0])*cos(deltaLon);
	float radAngle = atan2(y,x);
	int angle = radToDeg(radAngle);

	Serial.print(F("Needed heading: "));
	Serial.println(angle);
	return angle;
}
 
void dofAHRS::enable(bool enable) {
	ahrsEnabled = (int)enable;
}

bool dofAHRS::isEnabled() {
	return ahrsEnabled;
}

void dofAHRS::init() {
  if(accel.begin() && mag.begin() && bmp.begin()) {
    Serial.println(F("AHRS System Initialized"));
  }
  else {
    Serial.println(F("Failed to initialize AHRS system. Check wiring."));
  }
}
