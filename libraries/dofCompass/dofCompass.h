#include "Arduino.h"

class dofCompass {
	
	public:
		struct latLon {
			float north, west;
		};
		float getNeededHeading(latLon currLoc, latLon neededLoc), getCurrentHeading();
		void init(), enable(bool enable);
		bool isEnabled();
		int compassEnabled;
};