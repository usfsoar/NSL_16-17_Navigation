#include "Timer.h"
#include "Lander.h"
#include "comms.h"

Lander lander;
Timer failTimer;

float targetLoc[2] = {0, 0};

void setup() {
	Serial.begin(57600);
  initComms();
	delay(2000);
  
  while (!piInit()) //Wait for Pi to tell us to start
    delay(1000);

  float* latLon = lander.gps.getCurrentLatLon(); // Assuming we are close to tarps?
  targetLoc[0] = latLon[0];
  targetLoc[1] = latLon[1];

	lander.servos.setPin(1, 0); // pan servo
	lander.servos.setPin(2, 3); // tilt servo
	
	lander.init();
}

void loop() {	
  while (!isDeployed()) {
    lander.errorCheck();
    delay(5000);
  }
  
  while (isDeployed()) {
    lander.pointTo(targetLoc);
    if (needToShutdown()) {
      lander.servos.setAngle(1, 45);
      while (true)
        delay(500);
    }
  }
  
}
