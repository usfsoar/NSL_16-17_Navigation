#include "Timer.h"
#include "Lander.h"

Lander lander;
Timer failTimer;

float targetLoc[2] = {28.057848, -82.419760};

void setup() {
  Serial.begin(115200);
  delay(2000);
  lander.servos.setPin(1, 0);
  lander.servos.setPin(2, 1);
  lander.dof.enable(true);
  lander.dof.compass.enable(true);
  lander.dof.altimeter.setGroundPressure(101325);
  // Ground level (not sea level) pressure in Pa. Update daily, use barometer.
  lander.dof.altimeter.enable(true);
  lander.gps.enable(false);
  lander.servos.enable(true);
  lander.init();
  failTimer.start();
}

int loopNo = 0;

void loop() {  
  while (failTimer.getElapsedTime() < 1000) {
    loopNo++;
    lander.pointTo(targetLoc);
    Serial.println(F("---------------------NEW LOOP--------------------"));
    delay(200);
  }
}
