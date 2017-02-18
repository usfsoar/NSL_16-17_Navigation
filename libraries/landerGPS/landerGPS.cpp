#include "Arduino.h"
#include "landerGPS.h"

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#define GPSECHO  false 

SoftwareSerial mySerial(3, 2); //Adafruit_GPS GPS(&Serial1); -< using hdwr serial
Adafruit_GPS GPS(&mySerial);
bool usingInterrupt = false;
// void useInterrupt(boolean); 

uint32_t timer;
landerGPS::latLon cachedVal;
landerGPS::latLon nullLatLon;

/* SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
#endif
} */

/* void useInterrupt(boolean v) {
  if (v) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
} */

landerGPS::latLon landerGPS::getCurrentLatLon() {
  if (! usingInterrupt) {
    char c = GPS.read();
  }
  
  if (GPS.newNMEAreceived()) {  
    if (!GPS.parse(GPS.lastNMEA())) {
      return cachedVal; 
    }
  }
  if (timer > millis()) { 
    timer = millis();
  }

  if (millis() - timer > 2000) { 
    timer = millis();
    
    if (GPS.fix) {
      cachedVal.north = GPS.latitudeDegrees;
      cachedVal.west = GPS.longitudeDegrees;
      latLon currLatLon;
      currLatLon.north = GPS.latitudeDegrees;
      currLatLon.west = GPS.longitudeDegrees;
      return currLatLon;
    } else {
      Serial.println("No GPS Fix");
      return nullLatLon;
    }
  }
  return cachedVal;
}

void landerGPS::initGPS() {
  Serial.println("Initializing GPS");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  
  GPS.sendCommand(PGCMD_ANTENNA);

  // useInterrupt(true);
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE); 
  Serial.println("GPS Initialized"); 
}