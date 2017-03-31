#include "Arduino.h"
#include <Servo.h>
#include <landerGPS.h>
#include <landerDOF.h>
#include <landerServos.h>

class Lander {
	public:
		void pointTo(float targetLoc[2]);
		bool init();
		landerDOF dof;
		landerGPS gps;
		landerServos servos;
};