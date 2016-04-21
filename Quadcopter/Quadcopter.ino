#include <Wire.h>

#include <I2Cdev.h>
#include <MPU6050.h>

#include <Servo.h>
#include "PID.h"
#include "motor.h"
#include "sensors.h"

#define DEBUG_BAUD 115200
#define XBEE_BAUD 57600

//Current state of the copter
//Calculated in Sensors.cpp
//pitch and roll are states, but yaw is speed
double pitch = 0, yaw = 0, roll = 0;

double ch_lift = 0, ch_pitch = 0, ch_yaw = 0, ch_roll = 0;



int tps = 0; //Ticks per second
long ctime; //Time of the beginning of the current tick
long ptime; //Time of the beginning of the previous tick
double dt; //Time since the last tick in seconds
long clocktime = 0; //Used to count TPS, stores the time since the last TPS print
long lastcontroltime = -1; //Time since the last message from the controller in microseconds

/*PID pidPitch = PID(0,0,0);
PID pidYaw = PID(0,0,0);
PID pidRoll = PID(0,0,0);*/


void setup() {
  Serial.begin(9600);
  Sensors::init(0);

  /*ptime = micros();
  clocktime = ptime;
  Sensors::init(0);
  initDebug(DEBUG_BAUD);
  initComm(XBEE_BAUD);
  
  delay(1000); //JUST FOR TESTS*/
}

void loop() {
  Sensors::update();
  Serial.print("pitch=");
  Serial.print(Sensors::getPitch());
  Serial.print("\tyaw=");
  Serial.print(Sensors::getYaw());
  Serial.print("\troll=");
  Serial.println(Sensors::getRoll());

  /*//Time management
  ctime = micros();
  dt = (ctime-ptime)/1000000.0;
  if(ctime-clocktime >= 1000000){
    sendTPS(tps);
    debugTPS(tps);
    tps = 0;
    clocktime = ctime;
  }
  tps++;

  //Printing state in every 10th tick for debug reasons
  if((tps % 10) == 0 ){
    printPYR(pitch, yaw, roll);
  }
  
  Sensors::update();
  updateComm();


  ptime = ctime;*/
}

