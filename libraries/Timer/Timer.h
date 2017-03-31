#include "Arduino.h"


class Timer {
	public:
		void pause(), reset(), start(), restart();
		int64_t getElapsedTime();
		bool isRunning;
	private:
		int64_t startTime, totalRan;
};