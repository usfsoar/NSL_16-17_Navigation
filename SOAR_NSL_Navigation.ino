#include "Timer.h"
#include "Lander.h"

Lander lander;
Timer failTimer;

float targetLoc[2] = {28.057002, -82.428837};

void setup() {
  Serial.begin(115200);
  delay(2000);
  lander.servos.setPin(1, 0); // pan servo
  lander.servos.setPin(2, 3); // tilt servo
  lander.dof.enable(true);
  lander.dof.ahrs.enable(true);
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
  while (failTimer.getElapsedTime() < 300000) {
    loopNo++;
    lander.pointTo(targetLoc);
    Serial.println(F("---------------------NEW LOOP--------------------"));
    delay(100);
  }
}
