#include "Arduino.h"
#include "dofAltimeter.h"

int altimeterEnabled = -1;
float groundLevelPressure;

float dofAltimeter::degToRad(float deg) {
  return (deg * 3.14159265358979323846 / 180);
}

float dofAltimeter::getDistanceBetween(float locA[2], float locB[2]) {
  float radius = 6371.0; // Radius of Earth
  float radLocA[2], radLocB[2], dLoc[2];

  // Radians:
  radLocA[0] = degToRad(locA[0]); 
  radLocA[1] = degToRad(locA[1]);
  radLocB[0] = degToRad(locB[0]);
  radLocB[1] = degToRad(locB[1]);

  dLoc[0] = sin((radLocB[0] - radLocA[0])/2);
  dLoc[1] = sin((radLocB[1] - radLocA[1])/2);

  // Haversine (from http://stackoverflow.com/questions/10198985/calculating-the-distance-between-2-latitudes-and-longitudes-that-are-saved-in-a):
  float dist = 2.0 * radius * asin(sqrt(dLoc[0] * dLoc[0] + cos(radLocA[0]) * cos(radLocB[0]) * dLoc[1] * dLoc[1]));

  Serial.print(F("Distance calculated: "));
  Serial.println(dist);
  return dist; // Meters
}

int dofAltimeter::getNeededTiltAngle(float currLoc[2], float neededLoc[2], float altitude) {
  float dist = getDistanceBetween(currLoc, neededLoc); // Meters
  
  if (altitude < 0) {
    altitude = 0;
  }

  int angle = (int)atan2(altitude, dist) * 180 / 3.141592; // Degrees
  // angle should always be in first quadrant (0-90), as altitude and distance should always be positive

  if (angle > 45) {
    angle = 45;
  }

  Serial.print(F("Needed tilt angle: "));
  Serial.println(angle);
  return angle; //Degrees, range [0,45]
}



float dofAltimeter::getCurrentAltitude() {
  sensors_event_t bmp_event;
  bmp.getEvent(&bmp_event);

  if (bmp_event.pressure)
  {
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print(F("Current temperature: "));
    Serial.println(temperature);

    float altitude = bmp.pressureToAltitude(SENSORS_PRESSURE_SEALEVELHPA,
                                               bmp_event.pressure,
                                               temperature); 
    Serial.print(F("Current pressure: "));
    Serial.println(bmp_event.pressure);
    Serial.print(F("Current altitude: "));
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
  Serial.print(F("Ground pressure set to: "));
  Serial.println(val);
}
 
void dofAltimeter::enable(bool enable) {
	altimeterEnabled = (int)enable;
}

bool dofAltimeter::isEnabled() {
	return altimeterEnabled;
}

void dofAltimeter::init() {
  if(bmp.begin()) {
    Serial.println(F("Altimeter Initialized"));
  }
  else {
    Serial.println(F("Failed to initialize altimeter. Check wiring."));
  }
}
