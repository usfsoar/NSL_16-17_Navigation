#include "Arduino.h"


class Timer {
	public:
		void pause(), reset(), start(), restart();
		int64_t getElapsedTime();
	private:
		int64_t startTime, totalRan;
		bool isRunning;
};