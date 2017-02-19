#include "Arduino.h"
#include "landerCompass.h"


Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);
int compassEnabled = -1;

float landerCompass::getNeededHeading(landerCompass::latLon currLoc, landerCompass::latLon neededLoc) {
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

float landerCompass::getCurrentHeading() {
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;
  
  accel.getEvent(&accel_event);

  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    return orientation.heading;
  }
}  

void landerCompass::enable(bool enable) {
	compassEnabled = (int)enable;
}

bool landerCompass::isEnabled() {
	return compassEnabled;
}

void landerCompass::init() {
  Serial.println("Initializing 10 DOF board");
  
  if(!accel.begin() || !mag.begin() || !bmp.begin() || !gyro.begin()){
    Serial.println("Unable to start 10 DOF board");
    while (true) {
      delay(1000);  
    }
  }
  Serial.println("10 DOF board Initialized");
}


