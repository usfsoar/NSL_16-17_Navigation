#include "Arduino.h"
#include "Timer.h"

bool isRunning = false;

void Timer::pause() {
	if (!isRunning) {
		isRunning = false;
		totalRan += millis() - startTime;
	}
}

void Timer::reset() {
	totalRan = 0;
	startTime = totalRan;
	isRunning = false;
}

void Timer::start() {
	if (!isRunning) {
		isRunning = true;
		startTime = millis();
	}
}

void Timer::restart() {
	reset();
	start();
}

int64_t Timer::getElapsedTime() {
	if (isRunning)
		return (millis() - startTime);
	else
		return totalRan;
}




