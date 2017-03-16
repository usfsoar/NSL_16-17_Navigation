#include "Arduino.h"
#include <Servo.h>
#include <landerGPS.h>
#include <landerDOF.h>
#include <landerServos.h>

class Lander {
	public:
		struct latLon {
			float north, west;   
		};  
	
	public:
		void abort(), timeOut();
		void navigateTo(int reductionFactor, int heading);
		bool init();
		landerDOF dof;
		landerGPS gps;
		landerServos servos;
};