#include <Wire.h>
#include <math.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <Adafruit_GPS.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#define GPSECHO  false

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

SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);

boolean usingInterrupt = false;
void useInterrupt(boolean); 

float Pi = 3.14159;
uint32_t startTime, timer;


class latLon
{
  public:
   float north, west;   
};

latLon nullLatLon;

float getNeededHeading(float currHeading, latLon currLoc, latLon neededLoc) {
  float angle = degrees(atan2((neededLoc.north - currLoc.north), (neededLoc.west - currLoc.west)));
  Serial.print ("Calculated needed angle: "); Serial.println(angle);
  return currHeading - angle + 90;
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

SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

latLon getCurrentLatLon() {
  if (! usingInterrupt) {
    char c = GPS.read();
  }
  
  if (GPS.newNMEAreceived()) {  
    if (!GPS.parse(GPS.lastNMEA()))
      return nullLatLon; 
  }
  if (timer > millis()) { 
    timer = millis();
  }

  if (millis() - timer > 2000) { 
    timer = millis();
    
    if (GPS.fix) {
      latLon currLatLon;
      currLatLon.north = GPS.latitudeDegrees;
      currLatLon.west = GPS.longitudeDegrees;
      return currLatLon;
    } else {
      return nullLatLon;
    }
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
  Serial.println("INIT");
  
  if(!accel.begin() || !mag.begin() || !bmp.begin() || !gyro.begin()){
    Serial.println("Unable to start 10 DOF board");
    while (true) {
      delay(1000);  
    }
  }
}

void initGPS() {
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  
  GPS.sendCommand(PGCMD_ANTENNA);

  useInterrupt(true);
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);  
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

void setup()  {
  Serial.begin(115200);
  initDofBoard();
  initMotors();
  startTime = millis();
  timer = millis();
  nullLatLon.north = 0;
  nullLatLon.west = 0;
  int timer = millis();
}

float neededHeading = 90; // Static compass heading lander goes towards

void loop(){
  delay(20);
  runMotors(startMotorValue, 6, neededHeading);
}
