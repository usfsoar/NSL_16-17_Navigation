#include "Timer.h"
#include "Lander.h"

struct latLon {
   float north, west;   
};  

Lander lander;
Timer failTimer;

void setup() {
  Serial.begin(115200);
  lander.servos.setPin(1, 6);
  lander.servos.setPin(2, 7);
  lander.dof.enable(true);
  lander.dof.compass.enable(true);
  lander.dof.altimeter.setGroundPressure(102600);
  // Ground level (not sea level) pressure in Pa. Update daily, use barometer.
  lander.dof.altimeter.enable(true);
  lander.gps.enable(false);
  lander.servos.enable(true);
  failTimer.start();
}

void loop(){  
  while (failTimer.getElapsedTime() < 10 * 1000) {
    delay(20);
    //lander.navigateTo(4, 90);
    lander.pointTo();
  }
}
