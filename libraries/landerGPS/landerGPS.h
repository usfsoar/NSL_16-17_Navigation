#include "Arduino.h"

class landerGPS {
	
	public:
		struct latLon {
			public:
				float north, west;
		};
	
	public:
		void init();
		void enable(bool enable);
		bool isEnabled();
		int gpsEnabled;
		
	private:
		bool usingInterrupt;
		void useInterrupt();
		uint32_t timer;
		latLon cachedVal, nullLatLon, getCurrentLatLon();
};