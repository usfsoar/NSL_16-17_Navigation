#include "Arduino.h"

class landerGPS {
	
	public:
		void init();
		void enable(bool enable);
		bool isEnabled();
		int gpsEnabled;
		
	private:
		bool usingInterrupt;
		void useInterrupt();
		uint32_t timer;
		float cachedVal[2], nullLatLon[2], * getCurrentLatLon();
};