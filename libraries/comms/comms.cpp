#include "Arduino.h"
#include "comms.h"
#include "Wire.h"
#include <avr/pgmspace.h>

int reqVal, commAltitude, commDistance;
float commLon, commLat;
bool safeToStart = false, dofError, gpsError, gpsHasFix, shutdown, landerIsDeployed;

const PROGMEM int ALTREQONE = 0, ALTREQTWO = 1, ERRORCODEREQ = 2, DISTREQONE = 3, DISTREQTWO = 4, INITREQ = 5, INITDOFREQ = 6, INITGPSREQ = 7, GPSFIXREQ = 8, SHUTDOWN = 9;
const PROGMEM int LATREQONE = 10, LATREQTWO = 11, LATREQTHREE = 12, LATREQFOUR = 13;
const PROGMEM int LONREQONE = 14, LONREQTWO = 15, LONREQTHREE = 16, LONREQFOUR = 17, ISDEPLOYED = 18;

int getFirstAltByte() {
	return int(commAltitude / 100);
}

int getSecondAltByte() {
	return commAltitude - (getFirstAltByte() * 100);
}

int getFirstDistByte() {
	return int(commDistance / 100);
}

int getSecondDistByte() {
	return commDistance - (getFirstDistByte() * 100);
}

int getLatByte(int whichByte) {
	double lat = commLat;
	int theByte;
	for (int i = 0; i < whichByte; i++) {
		theByte = lat;
		lat -= theByte;
		lat *= 100;
	}
	return theByte;
}

int getLonByte(int whichByte) {
	double lon = commLon;
	int theByte;
	for (int i = 0; i < whichByte; i++) {
		theByte = lon;
		lon -= theByte;
		lon *= 100;
	}
	return theByte;
}

void receiveData(int byteCount){
  while(Wire.available())
    reqVal = Wire.read();
}

void sendData(){
	Serial.println("Here");
  if (reqVal == ALTREQONE) 
	  Wire.write(getFirstAltByte());
  if (reqVal == ALTREQTWO)
	  Wire.write(getSecondAltByte());
  
  if (reqVal == DISTREQONE) 
	  Wire.write(getFirstDistByte());
  if (reqVal == DISTREQTWO)
	  Wire.write(getSecondDistByte());
  
  if (reqVal == LATREQONE) 
	  Wire.write(getLatByte(1));
  if (reqVal == LATREQTWO)
	  Wire.write(getLatByte(2));
  if (reqVal == LATREQTHREE) 
	   Wire.write(getLatByte(3));
  if (reqVal == LATREQFOUR)
	  Wire.write(getLatByte(4));
 
   if (reqVal == LONREQONE) 
	  Wire.write(getLonByte(1));
  if (reqVal == LONREQTWO)
	  Wire.write(getLonByte(2));
  if (reqVal == LONREQTHREE) 
	   Wire.write(getLonByte(3));
  if (reqVal == LONREQFOUR)
	  Wire.write(getLonByte(4));
 
  if (reqVal == INITDOFREQ)
	  Wire.write(dofError);
  if (reqVal == INITGPSREQ)
	  Wire.write(gpsError);
 
   if (reqVal == GPSFIXREQ) 
	  Wire.write(gpsHasFix);
  
  if (reqVal == INITREQ) {
	  Wire.write(INITREQ);
	  safeToStart = true;
  }
  
    if (reqVal == SHUTDOWN) {
	  Wire.write(true);
	  shutdown = true;
  }
  
    if (reqVal == ISDEPLOYED) {
	  Wire.write(true);
	  landerIsDeployed = true;
  }
  
}

void setCommLat(float lat) {
	commLat = lat;
}

void setCommLon(float lon) {
	commLon = lon;
}

void setCommAltitude(int alt) {
	commAltitude = alt;
}

void setCommDistance(int dist) {
	commDistance = dist;
}

void setDofError(bool er) {
	dofError = er;
}

void setGpsHasFix(bool er) {
	gpsHasFix = er;
}

void setGpsError(bool er) {
	gpsError = er;
}

bool needToShutdown(){
	return shutdown;
}

bool isDeployed(){
	return landerIsDeployed;
}

bool piInit() {
	return safeToStart;
}

bool initComms() {
  pinMode(13, OUTPUT);
  Wire.begin(0X04);
  
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  return true;
}