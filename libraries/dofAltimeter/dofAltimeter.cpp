#include "Arduino.h"
#include "dofAltimeter.h"

int altimeterEnabled = -1;
float groundLevelPressure;

float dofAltimeter::getDistanceBetween(dofAltimeter::latLon locA, dofAltimeter::latLon locB) {
  float radius = 6371000f; // Radius of Earth
  dofAltimeter::latLon radLocA;
  dofAltimeter::latLon radLocB;
  dofAltimeter::latLon deltaLoc;

  // Radians:
  radLocA.north = locA.north * 3.141592 / 180; 
  radLocA.west = locA.west * 3.141592 / 180;
  radLocB.north = locB.north * 3.141592 / 180;
  radLocB.west = locB.west * 3.141592 / 180;

  deltaLoc.north = locB.north - locA.north;
  deltaLoc.west = locB.west - locA.west;

  // Haversine (from http://www.movable-type.co.uk/scripts/latlong.html):
  float a = sin(deltaLoc.north / 2) * sin(deltaLoc.north / 2) + cos(radLocA.north) * cos(radLocB.north) * sin(deltaLoc.west / 2) * sin(deltaLoc.west / 2);
  float c = 2f * atan2(sqrt(a), sqrt(1-a));
  float d = radius * c;

  return d; // Meters
}

float dofAltimeter::getNeededTiltAngle(dofAltimeter::latLon currLoc, dofAltimeter::latLon neededLoc, float altitude) {
  float dist = getDistanceBetween(currLoc, neededLoc); // Meters
  
  if (altitude < 0) {
    altitude = 0
  }

  float angle = atan2(altitude, dist) * 180 / 3.141592; // Degrees
  // angle should always be in first quadrant (0-90), as altitude and distance should always be positive

  if (angle > 45) {
    angle = 45;
  }

  return angle; //Degrees, range [0,45]
}



float dofAltimeter::getCurrentAltitude() {
  sensors_event_t bmp_event;
  bmp.getEvent(&bmp_event)

  if (bmp_event.pressure)
  {
    float temperature;
    bmp.getTemperature(&temperature);

    float altitude = bmp.pressureToAltitude(groundLevelPressure,
                                            bmp_event.pressure,
                                            temperature); 

    return altitude;
  }
}

void dofAltimeter::setGroundPressure(float val) {
  groundLevelPressure = val;
  /* NOTE: groundLevelPressure is NOT pressure at sea level;
   the camera aiming will work best when we use height above
   ground rather than the tradition alt. above sea level; esp.
   on flat ground like the launch site */
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
