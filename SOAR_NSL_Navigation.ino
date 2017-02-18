#include <Timer.h>
#include <landerGPS.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <Servo.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

int startMotorValue = 1250;

int leftMotorPin = 6;
int rightMotorPin = 7;
Servo leftMotor;
Servo rightMotor;

boolean rightMotorOn = false;
boolean leftMotorOn = false;

float Pi = 3.14159;
uint32_t startTime, timer1;

struct latLon
{
  public:
   float north, west;   
};

float getNeededHeading(latLon currLoc, latLon neededLoc) {
  latLon relativeLoc;
  relativeLoc.north = neededLoc.north - currLoc.north;
  relativeLoc.west = neededLoc.west - currLoc.west;

  float angle = atan2(relativeLoc.north, relativeLoc.west);
  if(angle < 0) {
    angle += 2 * Pi; 
  }
  angle = degrees(angle);
  angle = int(450 - angle) % int(360);
  return angle;
}

float getCurrentHeading() {
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;
  
  accel.getEvent(&accel_event);

  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    return orientation.heading;
  }
}   

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

void initDofBoard() {
  Serial.println("Initializing 10 DOF board");
  
  if(!accel.begin() || !mag.begin() || !bmp.begin() || !gyro.begin()){
    Serial.println("Unable to start 10 DOF board");
    while (true) {
      delay(1000);  
    }
  }
  Serial.println("10 DOF board Initialized");
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

void runMotors(int start, int reductionFactor, int heading){
  int difference = getCurrentHeading() - heading;
  
  if (difference < 0) {
    difference += 360;
  }

  if (difference <= 180) {
    turnRightMotorOn(start + (difference / reductionFactor));
    turnLeftMotorOn(start);
  } else {
    turnLeftMotorOn(start + ((360 - difference) / reductionFactor));
    turnRightMotorOn(start);
  }  
}

latLon neededLatLon;

void setup()  {
  Serial.begin(115200);
  initDofBoard();
  initMotors();
  neededLatLon.north = 28.048582; 
  neededLatLon.west = -82.410679;
}

void loop(){  
  failSafeTimer(20000);
  delay(20);
  runMotors(startMotorValue, 4, 90);
}
