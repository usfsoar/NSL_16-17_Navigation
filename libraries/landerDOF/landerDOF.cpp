#include "Arduino.h"
#include "landerDOF.h"

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_10DOF								tdof	= Adafruit_10DOF(); /* 'dof' is already in use, so use 'tdof' */
Adafruit_LSM303_Mag_Unified	 mag	 = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified			 bmp	 = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified			 gyro	= Adafruit_L3GD20_Unified(20);

Adafruit_Simple_AHRS 				ahrs(&accel, &mag);

float groundLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

int * landerDOF::getCurrentOrientation() {
	static int hpr[3];

	sensors_vec_t orientation;
	ahrs.getOrientation(&orientation);

	hpr[0] = (int)round(orientation.heading);
	hpr[1] = (int)round(orientation.pitch);
	hpr[2] = (int)round(orientation.roll);

	return hpr;
}

float landerDOF::getCurrentAltitude() {
	sensors_event_t bmp_event;
	bmp.getEvent(&bmp_event);

	if (bmp_event.pressure)
	{
		float temperature;
		bmp.getTemperature(&temperature);
		Serial.print(F("Current temperature: "));

		float altitude = bmp.pressureToAltitude(groundLevelPressure,
																						bmp_event.pressure,
																						temperature); 
		Serial.print(F("Current pressure: "));
		Serial.println(bmp_event.pressure);
		Serial.print(F("Current altitude: "));
		Serial.println(altitude);
		return altitude;
	} else {
		return 0.0;
	}
}

bool landerDOF::hasError() {
	return (!accel.begin() || !mag.begin() || !bmp.begin());
}

bool landerDOF::init() {
	if(!accel.begin() || !mag.begin() || !bmp.begin())
		return false;

	sensors_event_t bmp_event;
	bmp.getEvent(&bmp_event);

	if (bmp_event.pressure && bmp_event.pressure > 0) {
		groundLevelPressure = bmp_event.pressure;
		return true;
	} else {
		return false;
	}
}

