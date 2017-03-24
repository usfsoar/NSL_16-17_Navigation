#include "Arduino.h"
#include "dofCompass.h"

int compassEnabled = -1;

int dofCompass::getNeededHeading(float currLoc[2], float neededLoc[2]) {
  float relativeLoc[2];
  relativeLoc[0] = neededLoc[0] - currLoc[0];
  relativeLoc[1] = neededLoc[1] - currLoc[1];

  int angle = (int)atan2(relativeLoc[0], relativeLoc[1]);
  if(angle < 0) {
    angle += 2 * 3.14159; 
  }
  angle = degrees(angle);
  angle = int(450 - angle) % int(360);

  Serial.print("Needed heading: ");
  Serial.println(angle);
  return angle;
}

int dofCompass::getCurrentHeading() {
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  
  accel.getEvent(&accel_event);

  mag.getEvent(&mag_event);
  if (tdof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    Serial.print("Current heading: ");
    Serial.println(orientation.heading);
    return (int)orientation.heading;
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
