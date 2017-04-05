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
  bool isInRocket = false, isDeployed = false;
  
  while (!isInRocket) {
    lander.errorCheck();
    delay(5000);
  }
  
  while (isInRocket && !isDeployed)
    delay(500);

  while (!isInRocket && isDeployed) {
    //pointTo
    if (needToShutdown()) {
      //setServos
      while (true)
        delay(500);
    }
  }
  
}
