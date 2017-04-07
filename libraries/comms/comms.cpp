#include "Arduino.h"
#include "comms.h"
#include "Wire.h"
#include <avr/pgmspace.h>

int reqVal, commAltitude, commDistance;
float commLon, commLat, commOZero, commOOne, commOTwo;
bool safeToStart = false, dofError, gpsError, gpsHasFix, shutdown, landerIsDeployed;

const PROGMEM int ALTREQ = 0, ERRORCODEREQ = 1, DISTREQ = 2, INITREQ = 3, INITDOFREQ = 4, INITGPSREQ = 5, GPSFIXREQ = 6, SHUTDOWNREQ = 7;
const PROGMEM int LATREQ = 8, LONREQ = 9, ISDEPLOYEDREQ = 10, ORIENTATIONSREQ = 11;

void sendFloat(float val) {
  String str = String(val, 9); //Carry 9 digits
  int strLen = str.length() + 1; 
  char cArr[strLen];
  str.toCharArray(cArr, strLen);
  for (int i = 0; i < strLen; ++i)
    Wire.write(int(cArr[i]));   
}

void receiveData(int byteCount){
  while(Wire.available())
    reqVal = Wire.read();
}

void sendData(){
  if (reqVal == ALTREQ)
	sendFloat(commAltitude);
  
  if (reqVal == DISTREQ) 
	sendFloat(commDistance);
  
  if (reqVal == LATREQ) 
	sendFloat(commLat);
 
  if (reqVal == LONREQ) 
	sendFloat(commLon);
 
  if (reqVal == INITDOFREQ)
	Wire.write(dofError);
 
  if (reqVal == GPSFIXREQ) 
	Wire.write(gpsHasFix);
  
  if (reqVal == INITREQ) {
	Wire.write(true);
	safeToStart = true;
  }
  
  if (reqVal == SHUTDOWNREQ) {
	  Wire.write(true);
	  shutdown = true;
  }
  
  if (reqVal == ISDEPLOYEDREQ) {
	  Wire.write(true);
	  landerIsDeployed = true;
  }
  
  if (reqVal == ORIENTATIONSREQ) {
	  sendFloat(commOZero);
	  sendFloat(commOOne);
	  sendFloat(commOTwo);
  }
  
}

void setCommOrientation(float zero, float one, float two) {
	commOZero = zero; commOOne = one; commOTwo = two;
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