#include "Timer.h"
#include "Lander.h"

Lander lander;
Timer failTimer;

float targetLoc[2] = {28.061608, -82.426095};

void setup() {
	Serial.begin(57600);
  initComms();
	delay(2000);
  
  while (!piInit()) //Wait for Pi to tell us to start
    delay(1000);

	lander.servos.setPin(1, 0); // pan servo
	lander.servos.setPin(2, 3); // tilt servo
	
	lander.init();
}

void run() {
  
}

void loop() {	
  bool isDeployed = false;
  
  while (!isDeployed) {
    lander.errorCheck();
    delay(500);
  }
  //
  lander.dof.getCurrentAltitude();
  delay(500);
}
