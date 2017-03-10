#include "Arduino.h"
#include "dofCompass.h"

int compassEnabled = -1;

float dofCompass::getNeededHeading(dofCompass::latLon currLoc, dofCompass::latLon neededLoc) {
  latLon relativeLoc;
  relativeLoc.north = neededLoc.north - currLoc.north;
  relativeLoc.west = neededLoc.west - currLoc.west;

  float angle = atan2(relativeLoc.north, relativeLoc.west);
  if(angle < 0) {
    angle += 2 * 3.14159; 
  }
  angle = degrees(angle);
  angle = int(450 - angle) % int(360);
  return angle;
}

float dofCompass::getCurrentHeading() {
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;
  
  accel.getEvent(&accel_event);

  mag.getEvent(&mag_event);
  if (tdof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    return orientation.heading;
  }
}  

void dofCompass::enable(bool enable) {
	compassEnabled = (int)enable;
}

bool dofCompass::isEnabled() {
	return compassEnabled;
}

void dofCompass::init() {
  Serial.println("Compass Initialized");
}
