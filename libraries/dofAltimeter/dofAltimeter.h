#include "Arduino.h"

class dofAltimeter {
	
	public:
		struct latLon {
			float north, west;
		};
		float getNeededTiltAngle(latLon currLoc, latLon neededLoc, float altitude), getCurrentAltitude(float groundLevelPressure);
		void init(), enable(bool enable);
		bool isEnabled();
		int altimeterEnabled;
};