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

int * dofAHRS::getCompensatedAngles(int hpr[3], int tilt, int pan) {
	static int compAngles[2];
	float alpha = degToRad(pan), beta = degToRad(-(90 - tilt)), psi = degToRad(hpr[0]), theta = degToRad(hpr[1]), phi = degToRad(hpr[2]);
	//float compBeta = asin(-sin(theta)*cos(beta)*cos(alpha) + cos(theta)*sin(phi)*cos(beta)*sin(alpha) + cos(theta)cos(phi)sin(beta));
	//float compAlpha = acos((cos(psi)*cos(theta)*cos(beta)*cos(alpha) + cos(psi)sin(theta)sin(phi)sin(alpha) - sin(psi)cos(phi)cos(beta)sin(alpha) + cos(psi)sin(theta)cos(phi)sin(beta) - sin(psi)sin(theta)sin(beta))/compBeta)

	//compAngles[0] = radToDeg(compAlpha);
	//compAngles[1] = -radToDeg(compBeta);

	compAngles[0] = tilt;
	compAngles[1] = pan;

	return compAngles;
}

/*int dofAHRS::getNeededHeading(float currLoc[2], float neededLoc[2]) {
	float relativeLoc[2];
	relativeLoc[0] = neededLoc[0] - currLoc[0]; //latitude
	relativeLoc[1] = neededLoc[1] - currLoc[1]; //longitude

	float radAngle = atan2(relativeLoc[0], relativeLoc[1]);

	int angle = radToDeg(angle);
	if(angle < 0) {
		angle += 360; 
	}
	angle = int(450 - angle) % int(360);

	Serial.print(F("Needed heading: "));
	Serial.println(angle);
	return angle;
}*/

int dofAHRS::getNeededHeading(float currLoc[2], float neededLoc[2]) {
	float deltaLon = neededLoc[1] - currLoc[1]; //longitude
	float y = sin(deltaLon) * cos(neededLoc[0]);
	float x = cos(currLoc[0])*sin(neededLoc[0]) - sin(currLoc[0])*cos(neededLoc[0])*cos(deltaLon);
	float radAngle = atan2(y,x);
	int angle = radToDeg(radAngle);// - 90;
	/*if(angle < -180) {
		angle += 180; 
	}*/

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
