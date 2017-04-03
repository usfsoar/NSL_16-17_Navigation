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

	if (ahrs.getOrientation(&orientation)) {
		/* 'orientation' should have valid .roll and .pitch fields */
		Serial.print(F("Orientation: "));
		Serial.print(orientation.heading);
		Serial.print(F(" "));
		Serial.print(orientation.pitch);
		Serial.print(F(" "));
		Serial.println(orientation.roll);
	} else {
		Serial.print(F("ERROR: Did not recieve AHRS event."));
	}

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
		Serial.println(temperature);

		float altitude = bmp.pressureToAltitude(groundLevelPressure,
																						bmp_event.pressure,
																						temperature); 
		Serial.print(F("Current pressure: "));
		Serial.println(bmp_event.pressure);
		Serial.print(F("Current altitude: "));
		Serial.println(altitude);
		return altitude;
	} else {
		Serial.print(F("ERROR: did not recieve pressure event."));
		return 0.0;
	}
}

bool landerDOF::init() {
	Serial.println(F("Initializing 10 DOF board..."));
	if(!accel.begin() || !mag.begin() || !bmp.begin()){
		Serial.println(F("ERROR: Unable to start 10 DOF board."));
		return false;
	} else {
		Serial.println(F("10 DOF board Initialized. Setting ground pressure..."));
	}

	sensors_event_t bmp_event;
	bmp.getEvent(&bmp_event);

	if (bmp_event.pressure && bmp_event.pressure > 0) {
		groundLevelPressure = bmp_event.pressure;
		Serial.print(F("GLP set to "));
		Serial.println(bmp_event.pressure);
		return true;
	} else {
		Serial.println(F("ERROR: did not recieve ground pressure event or recieved invalid value."));
		return false;
	}
}


