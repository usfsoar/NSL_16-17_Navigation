#include "Arduino.h"
#include "dofAltimeter.h"

int altimeterEnabled = -1;
float groundLevelPressure;

float dofAltimeter::getDistanceBetween(float locA[2], float locB[2]) {
  float radius = 6371000; // Radius of Earth
  float radLocA[2];
  float radLocB[2];
  float deltaLoc[2];

  // Radians:
  radLocA[0] = locA[0] * 3.141592 / 180; 
  radLocA[1] = locA[1] * 3.141592 / 180;
  radLocB[0] = locB[0] * 3.141592 / 180;
  radLocB[1] = locB[1] * 3.141592 / 180;

  deltaLoc[0] = locB[0] - locA[0];
  deltaLoc[1] = locB[1] - locA[1];

  // Haversine (from http://www.movable-type.co.uk/scripts/latlong.html):
  float a = sin(deltaLoc[0] / 2) * sin(deltaLoc[0] / 2) + cos(radLocA[0]) * cos(radLocB[0]) * sin(deltaLoc[1] / 2) * sin(deltaLoc[1] / 2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  float d = radius * c;

  Serial.print("Distance calculated: ");
  Serial.println(d);
  return d; // Meters
}

int dofAltimeter::getNeededTiltAngle(float currLoc[2], float neededLoc[2], int altitude) {
  float dist = getDistanceBetween(currLoc, neededLoc); // Meters
  
  if (altitude < 0) {
    altitude = 0;
  }

  int angle = (int)atan2(altitude, dist) * 180 / 3.141592; // Degrees
  // angle should always be in first quadrant (0-90), as altitude and distance should always be positive

  if (angle > 45) {
    angle = 45;
  }

  Serial.print("Needed tilt angle: ");
  Serial.println(angle);
  return angle; //Degrees, range [0,45]
}



int dofAltimeter::getCurrentAltitude() {
  sensors_event_t bmp_event;
  bmp.getEvent(&bmp_event);

  if (bmp_event.pressure)
  {
    float temperature;
    bmp.getTemperature(&temperature);

    int altitude = (int)bmp.pressureToAltitude(groundLevelPressure,
                                            bmp_event.pressure,
                                            temperature); 
    Serial.print("Current altitude: ");
    Serial.println(altitude);
    return altitude;
  }


}

void dofAltimeter::setGroundPressure(float val) {
  groundLevelPressure = val;
  /* NOTE: groundLevelPressure is NOT pressure at sea level;
   the camera aiming will work best when we use height above
   ground rather than the tradition alt. above sea level; esp.
   on flat ground like the launch site */
  Serial.print("Ground pressure set to: ");
  Serial.println(val);
}
 
void dofAltimeter::enable(bool enable) {
	altimeterEnabled = (int)enable;
}

bool dofAltimeter::isEnabled() {
	return altimeterEnabled;
}

void dofAltimeter::init() {
  Serial.println("Altimeter Initialized");
}
