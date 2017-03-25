#include "Arduino.h"
#include "Adafruit_LSM303_U.h"
#include "Adafruit_BMP085_U.h"
#include "Adafruit_L3GD20_U.h"
#include "Adafruit_10DOF.h"

class dofAltimeter {
	
	public:
		int getNeededTiltAngle(float currLoc[2], float neededLoc[2], float altitude);
		float getDistanceBetween(float locA[2], float locB[2]), getCurrentAltitude(), degToRad(float deg);
		void setGroundPressure(float val), init(), enable(bool enable);
		bool isEnabled();
		int altimeterEnabled;

	protected:
		Adafruit_LSM303_Accel_Unified accel;
		Adafruit_10DOF tdof;
		Adafruit_LSM303_Mag_Unified mag;
		Adafruit_BMP085_Unified bmp;
		Adafruit_L3GD20_Unified gyro;
};