#include "Arduino.h"
#include "landerDOF.h"

//dofCompass compass;
dofAltimeter altimeter;
dofAHRS ahrs;

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_10DOF                tdof  = Adafruit_10DOF(); /* 'dof' is already in use, so use 'tdof' */
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

bool landerDOF::init() {
  Serial.println(F("Initializing 10 DOF board..."));
  if(!altimeter.init() || !ahrs.init()){
    Serial.println(F("Unable to start 10 DOF board."));
    return false;
  } else {
    Serial.println(F("10 DOF board Initialized."));
    return true;
  }
}


