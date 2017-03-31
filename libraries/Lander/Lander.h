#include "Arduino.h"
#include <Servo.h>
#include <landerGPS.h>
#include <landerDOF.h>
#include <landerServos.h>

class Lander {
	public:
		float degToRad(int deg), degToRadFloat(float deg);
		int radToDeg(float rad);

		int getNeededHeading(float currLoc[2], float neededLoc[2]);
		float getDistanceBetween(float locA[2], float locB[2]);

		int * getCompensatedAngles(int hpr[3], float alt, float currentLoc[2], float targetLoc[2]);

		void pointTo(float targetLoc[2]);

		bool init();
		
		landerDOF dof;
		landerGPS gps;
		landerServos servos;
};