#include <Wire.h>

#include <I2Cdev.h>
#include <MPU6050.h>

#include <Servo.h>
#include "PID.cpp"

//Current state of the copter
//Calculated in Sensors.cpp
//pitch and roll are integrals, but yaw is speed
double pitch = 0,yaw = 0,roll = 0;

double ch_lift = 0, ch_pitch = 0, ch_yaw = 0, ch_roll = 0;


double dt;
long ctime, ptime, clocktime, lastcontroltime = -1;

PID pidPitch = PID(0,0,0);
PID pidYaw = PID(0,0,0);
PID pidRoll = PID(0,0,0);


void setup() {


  ptime = millis();
}

void loop() {
  ctime = millis();
  //Possibly needs revision
  dt = (ctime-ptime)/1000000.0;


  ptime = ctime;
}

