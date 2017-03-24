#include "Timer.h"
#include "Lander.h"

Lander lander;
Timer failTimer;

float targetLoc[2] = {0,0};

void setup() {
  Serial.begin(115200);
  delay(2000);
  lander.servos.setPin(1, 0);
  lander.servos.setPin(2, 1);
  lander.dof.enable(true);
  lander.dof.compass.enable(true);
  lander.dof.altimeter.setGroundPressure(102600);
  // Ground level (not sea level) pressure in Pa. Update daily, use barometer.
  lander.dof.altimeter.enable(true);
  lander.gps.enable(false);
  lander.servos.enable(true);
  lander.init();
  failTimer.start();
}

void loop(){  
  while (failTimer.getElapsedTime() < 10 * 1000) {
    delay(2000);
    //lander.navigateTo(4, 90);
    lander.pointTo(targetLoc);
  }
}
