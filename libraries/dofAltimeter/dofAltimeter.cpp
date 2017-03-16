#include "Arduino.h"
#include "dofAltimeter.h"

int altimeterEnabled = -1;
float groundLevelPressure;

float dofAltimeter::getNeededTiltAngle(dofAltimeter::latLon currLoc, dofAltimeter::latLon neededLoc, float altitude) {
  /* Range from 0 (straight down) to 90 (perpendicular) */
  float dist = sqrt(pow(2.0f, (currLoc.north - relativeLoc.north)) + pow(2.0f, (currLoc.west - relativeLoc.west)));
  
  if (altitude < 0)
  {
    altitude = 0
  }

  float angle = degrees(atan2(altitude, dist));
  /* angle should always be in first quadrant (0-90), as altitude and distance should always be positive */

  return angle;
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
