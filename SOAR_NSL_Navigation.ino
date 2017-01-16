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

void turnLeftOn() {
  if (!leftMotorOn) {
    leftMotor.writeMicroseconds(1250);
    leftMotorOn = true;
  }
}

void turnLeftOff() {
  if (leftMotorOn) {
    leftMotor.writeMicroseconds(1245);
    leftMotorOn = false;
  }
}

void turnRightOn() {
  if (!rightMotorOn){
    rightMotor.writeMicroseconds(1250);
    rightMotorOn = true;
  }
}

void turnRightOff() {
  if (rightMotorOn) {
    rightMotor.writeMicroseconds(1245);
    rightMotorOn = false;
  }
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
  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);
  leftMotor.writeMicroseconds(1100);
  rightMotor.writeMicroseconds(1100);
  delay(10000);  
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

void loop(){
  if (millis() - timer > 30000) { // Shuts motors off after 30 seconds
    turnRightOff();
    turnLeftOff();
    delay(500000000);
  }

  float neededHeading = 180.0; // Static heading lander goes towards

  if ((millis() - startTime) < 30000) { // Only when < 30 seconds
    if (getCurrentHeading() > neededHeading) {
      Serial.println("Left");
      turnLeftOn();
      turnRightOff();
    } else {    
      Serial.println("Right");
      turnRightOn();
      turnLeftOff();
    }
  }
}
