#include "Arduino.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"
#include "Adafruit_BMP085_U.h"
#include "Adafruit_L3GD20_U.h"
#include "Adafruit_10DOF.h"
#include "Adafruit_Simple_AHRS.h"

class dofAHRS {
	
	public:
		int * getCurrentOrientation(), 
			* getCompensatedAngles(int hpr[3], float alt, int panAngle, float currentLoc[2], float targetLoc[2]),
			getNeededHeading(float currLoc[2], float neededLoc[2]),
			radToDeg(float rad);
		float degToRad(int deg), degToRadFloat(float deg), getDistanceBetween(float locA[2], float locB[2]);
		void init(), enable(bool enable);
		bool isEnabled();
		int ahrsEnabled;

	protected:
		Adafruit_LSM303_Accel_Unified accel;
		Adafruit_10DOF tdof;
		Adafruit_LSM303_Mag_Unified mag;
		Adafruit_BMP085_Unified bmp;
		Adafruit_L3GD20_Unified gyro;
};