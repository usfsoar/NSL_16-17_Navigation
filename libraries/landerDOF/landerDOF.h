#include "Arduino.h"
#include "Adafruit_LSM303_U.h"
#include "Adafruit_BMP085_U.h"
#include "Adafruit_L3GD20_U.h"
#include "Adafruit_10DOF.h"
//#include <dofCompass.h>
#include <dofAltimeter.h>
#include <dofAHRS.h>

class landerDOF {
	
	public:
		bool init();
		dofAltimeter altimeter;
		dofAHRS ahrs;

	protected:
		Adafruit_LSM303_Accel_Unified accel;
		Adafruit_10DOF tdof;
		Adafruit_LSM303_Mag_Unified mag;
		Adafruit_BMP085_Unified bmp;
		Adafruit_L3GD20_Unified gyro;
};