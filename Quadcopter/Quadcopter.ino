#include <Wire.h>

#include <I2Cdev.h>
#include <MPU6050.h>

#include <Servo.h>

/*#include "Comm.cpp"
#include "Debug.cpp"
#include "Motors.cpp"
#include "PID.cpp"
#include "Sensors.cpp"*/

#define DEBUG_BAUD 115200

//Current state of the copter
//Calculated in Sensors.cpp
//pitch and roll are integrals, but yaw is speed
double pitch = 0,yaw = 0,roll = 0;

double ch_lift = 0, ch_pitch = 0, ch_yaw = 0, ch_roll = 0;


double dt;
long ctime, ptime, clocktime, lastcontroltime = -1;

/*PID pidPitch = PID(0,0,0);
PID pidYaw = PID(0,0,0);
PID pidRoll = PID(0,0,0);*/


void setup() {


  ptime = millis();
  initMPU();
  initDebug(DEBUG_BAUD);
}

bool blink = true;

void loop() {
  if(blink) digitalWrite(13,HIGH);
  else digitalWrite(13,LOW);
  blink = !blink;
  ctime = millis();
  readMPU(&pitch,&yaw,&roll,dt);
  printPYR(pitch,yaw,roll);
  //Possibly needs revision
  dt = (ctime-ptime)/1000000.0;


  ptime = ctime;
}

