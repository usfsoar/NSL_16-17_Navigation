#include <Timer.h>
#include <landerGPS.h>
#include <landerCompass.h>
#include <Servo.h>

int startMotorValue = 1250;

int leftMotorPin = 6;
int rightMotorPin = 7;
Servo leftMotor;
Servo rightMotor;

boolean rightMotorOn = false;
boolean leftMotorOn = false;

uint32_t startTime, timer1;

struct latLon {
  public:
   float north, west;   
};  

void turnLeftMotorOn(int val) {
  leftMotor.writeMicroseconds(val);
}

void turnLeftMotorOff() {
  leftMotor.writeMicroseconds(1100);
}

void turnRightMotorOn(int val) {
  rightMotor.writeMicroseconds(val);
}

void turnRightMotorOff() {
  rightMotor.writeMicroseconds(1100);
}

void initMotors() {
  Serial.println("Initializing Motors");
  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);
  turnRightMotorOff();
  turnLeftMotorOff();
  delay(10000);  
  Serial.println("Motors Initialized");
  turnRightMotorOn(startMotorValue);
  turnLeftMotorOn(startMotorValue);
}

void failSafeTimer(int timeToShutOffMS) {
  if (millis() - timer1 > timeToShutOffMS) {
    turnRightMotorOff();
    turnLeftMotorOff();
    delay(100000);
  }
}

//void runMotors(int start, int reductionFactor, int heading){
//  int difference = getCurrentHeading() - heading;
//  
//  if (difference < 0) {
//    difference += 360;
//  }
//
//  if (difference <= 180) {
//    turnRightMotorOn(start + (difference / reductionFactor));
//    turnLeftMotorOn(start);
//  } else {
//    turnLeftMotorOn(start + ((360 - difference) / reductionFactor));
//    turnRightMotorOn(start);
//  }  
//}

latLon neededLatLon;

void setup()  {
  Serial.begin(115200);
  initMotors();
  neededLatLon.north = 28.048582; 
  neededLatLon.west = -82.410679;
}

void loop(){  
  failSafeTimer(20000);
  delay(20);
  //runMotors(startMotorValue, 4, 90);
}
