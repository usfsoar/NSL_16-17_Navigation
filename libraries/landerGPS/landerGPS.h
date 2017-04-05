#include "Arduino.h"

class landerGPS {
	
	public:
		bool init(), hasFix(), hasError();
		float * getCurrentLatLon();
		
	private:
		bool usingInterrupt;
		void useInterrupt();
		uint32_t timer;
		float cachedVal[2], nullLatLon[2];
};