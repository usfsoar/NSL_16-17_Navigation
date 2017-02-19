#include "Arduino.h"
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

class landerCompass {
	
	public:
		struct latLon {
			float north, west;
		};
		float getNeededHeading(latLon currLoc, latLon neededLoc), getCurrentHeading();
		void init(), enable(bool enable);
		bool isEnabled();
		int compassEnabled;

	private:
		Adafruit_LSM303_Accel_Unified accel;
		Adafruit_10DOF dof;
		Adafruit_LSM303_Mag_Unified mag;
		Adafruit_BMP085_Unified bmp;
		Adafruit_L3GD20_Unified gyro;
};