#include "Timer.h"
#include "Lander.h"

struct latLon {
   float north, west;   
};  

latLon neededLatLon;
Lander lander;
Timer failTimer;

void setup()  {
  Serial.begin(115200);
  lander.motors.setLeftPin(6);
  lander.motors.setRightPin(7);
  lander.motors.setMinVal(1250);
  lander.dof.enable(true);
  lander.dof.compass.enable(true);
  lander.dof.altimeter.enable(true);
  lander.gps.enable(false);
  lander.motors.enable(true);
  lander.motors.setMinVal(1250);
  failTimer.start();
}

void loop(){  
  while (failTimer.getElapsedTime() < 10 * 1000) {
    delay(20);
    lander.navigateTo(4, 90);
  }
}
