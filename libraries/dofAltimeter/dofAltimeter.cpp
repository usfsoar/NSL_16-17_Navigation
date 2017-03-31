#include "Arduino.h"
#include "dofAltimeter.h"

float groundLevelPressure;

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
  } else {
    Serial.print(F("ERROR: did not recieve prssure event."));
    return 0.0;
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

bool dofAltimeter::init() {
  if(bmp.begin()) {
    Serial.println(F("Altimeter Initialized"));
    return true;
  }
  else {
    Serial.println(F("Failed to initialize altimeter. Check wiring."));
    return false;
  }
}
