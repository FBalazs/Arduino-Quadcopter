#include "sensors.h"
#include "comm.h"
#include "motor.h"
#include "PID.h"

int tps = 0; //Ticks per second
long ctime; //Time of the beginning of the current tick
long ptime; //Time of the beginning of the previous tick
double dt; //Time since the last tick in seconds
long clocktime = 0; //Used to count TPS, stores the time since the last TPS print

void setup() {
  ptime = micros();
  clocktime = ptime;
  
  Sensors::init(0);
  Comm::init(57600);
  
  delay(1000);
}

void loop() {
  Sensors::update();

  //Time management
  ctime = micros();
  dt = (ctime-ptime)/1000000.0;
  if(ctime-clocktime >= 1000000){
    Comm::sendTPS(tps);
    tps = 0;
    clocktime = ctime;
  }
  tps++;

  //Printing state in every 10th tick for debug reasons
  if(true && (tps % 550) == 0 ){
    Comm::print("pitch=");
    Comm::print(String(Sensors::getPitch()));
    Comm::print("\tyaw=");
    Comm::print(String(Sensors::getYaw()));
    Comm::print("\troll=");
    Comm::print(String(Sensors::getRoll()));
    Comm::print("\n");
  }
  
  Sensors::update();
  Comm::update();

  ptime = ctime;
}

