#include "Arduino.h"

class dofAltimeter {
	
	public:
		struct latLon {
			float north, west;
		};
		float getNeededTiltAngle(latLon currLoc, latLon neededLoc, float altitude), getCurrentAltitude();
		void setGroundPressure(float val), init(), enable(bool enable);
		bool isEnabled();
		int altimeterEnabled;
};