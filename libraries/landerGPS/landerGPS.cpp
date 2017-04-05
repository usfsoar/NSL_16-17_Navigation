#include "Arduino.h"
#include "landerGPS.h"

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#define GPSECHO	true

SoftwareSerial mySerial(3, 4); //Adafruit_GPS GPS(&Serial1); -< using hdwr serial
Adafruit_GPS GPS(&mySerial);

uint32_t timer = millis();


bool landerGPS::hasFix() {
	return GPS.fix;
}

bool landerGPS::hasError(){
	return false;
}

float * landerGPS::getCurrentLatLon() {
	static float nullLatLon[2];
	static float currLatLon[2];
	
	char c = GPS.read();

	if (GPS.newNMEAreceived()) {	
		if (!GPS.parse(GPS.lastNMEA())) {
			return nullLatLon; 
		}
	}
	if (timer > millis()) { 
		timer = millis();
	}

	if (millis() - timer > 2000) 
		timer = millis();
		
		if (GPS.fix) {
			currLatLon[0] = GPS.latitudeDegrees;
			currLatLon[1] = GPS.longitudeDegrees;
			return currLatLon;
		} else {
			Serial.println(F("No GPS Fix"));
			return nullLatLon;
		}
}

bool landerGPS::init() {
	GPS.begin(9600);
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);	
	GPS.sendCommand(PGCMD_ANTENNA);
	delay(1000);
	mySerial.println(PMTK_Q_RELEASE); 
	return true;
}