#include "Timer.h"
#include "Lander.h"

Lander lander;
Timer failTimer;

float targetLoc[2] = {28.061608, -82.426095};

void setup() {
  Serial.begin(57600);
  delay(2000);

  lander.servos.setPin(1, 0); // pan servo
  lander.servos.setPin(2, 3); // tilt servo
  // Ground level (not sea level) pressure in Pa. Update daily, use barometer.
  lander.dof.setGroundPressure(101325);
  
  if(!lander.init()) {
    Serial.println(F("ERROR: Lander failed to initialize. Quitting."));
  } else {
    Serial.println(F("Succesfully initialized lander. Starting."));
    failTimer.start();
  }
  delay(5000);
}

void loop() {  
  while (failTimer.getElapsedTime() < 300000 && failTimer.isRunning) {
    Serial.println(F("--------------------- NEW LOOP ---------------------"));
    lander.pointTo(targetLoc);
    delay(50);
  }
}
