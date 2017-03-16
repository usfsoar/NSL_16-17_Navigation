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
  lander.servos.setPin("pan", 6);
  lander.servos.setPin("tilt", 7);
  lander.dof.enable(true);
  lander.dof.compass.enable(true);
  lander.dof.altimeter.enable(true);
  lander.gps.enable(false);
  lander.motors.enable(true);
  failTimer.start();
}

void loop(){  
  while (failTimer.getElapsedTime() < 10 * 1000) {
    delay(20);
    //lander.navigateTo(4, 90);
  }
}
