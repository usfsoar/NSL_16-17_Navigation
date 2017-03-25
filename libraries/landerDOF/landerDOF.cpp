#include "Arduino.h"
#include "landerDOF.h"

dofCompass compass;
dofAltimeter altimeter;

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_10DOF                tdof  = Adafruit_10DOF(); /* 'dof' is already in use, so use 'tdof' */
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);
int dofEnabled = -1;

void landerDOF::enable(bool enable) {
  dofEnabled = (int)enable;
}

bool landerDOF::isEnabled() {
  return dofEnabled;
}

void landerDOF::init() {
  if (compass.compassEnabled == -1 || altimeter.altimeterEnabled == -1) {
    Serial.println(F("All DOF sensors must be enabled or disabled."));
    while (true) {
      delay(1000);  
    }
  }

  Serial.println(F("Initializing 10 DOF board..."));
  
  if(!accel.begin() || !mag.begin() || !bmp.begin() || !gyro.begin()){
    Serial.println(F("Unable to start 10 DOF board."));
    while (true) {
      delay(1000);  
    }
  }
  
  Serial.println(F("10 DOF board Initialized."));

  if (compass.isEnabled())
    compass.init();
  if (altimeter.isEnabled())
    altimeter.init();
}


