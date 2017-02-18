#include "Arduino.h"

class landerGPS {
	
	public:
		struct latLon {
			public:
				float north, west;
		};
	
	private:
		bool usingInterrupt;
		void useInterrupt(), initGPS();
		uint32_t timer;
		latLon cachedVal, nullLatLon, getCurrentLatLon();
};