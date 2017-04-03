#include "Arduino.h"
#include "comms.h"
#include "Wire.h"

int reqVal, commAltitude, commDistance;
bool safeToStart = false, dofError, gpsError, gpsHasFix;

const int ALTREQONE = 0, ALTREQTWO = 1, ERRORCODEREQ = 2, DISTREQONE = 3, DISTREQTWO = 4, INITREQ = 5, INITDOFREQ = 6, INITGPSREQ = 7, GPSFIXREQ = 8;

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

void receiveData(int byteCount){
  while(Wire.available())
    reqVal = Wire.read();
}

void sendData(){
  if (reqVal == ALTREQONE) 
	  Wire.write(getFirstAltByte());
  if (reqVal == ALTREQTWO)
	  Wire.write(getSecondAltByte());
  
  if (reqVal == DISTREQONE) 
	  Wire.write(getFirstDistByte());
  if (reqVal == DISTREQTWO)
	  Wire.write(getSecondDistByte());
 
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

void setgpsHasFix(bool er) {
	gpsHasFix = er;
}

void setGpsError(bool er) {
	gpsError = er;
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