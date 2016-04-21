#include "sensors.h"
#include "comm.h"
#include "motor.h"
#include "PID.h"

int tps = 0; //Ticks per second
long ctime; //Time of the beginning of the current tick
long ptime; //Time of the beginning of the previous tick
double dt; //Time since the last tick in seconds
long clocktime = 0; //Used to count TPS, stores the time since the last TPS print

PID pidPitch(90.0,0.0,0.0);
PID pidYaw(0.0,0.0,0.0);
PID pidRoll(90.0,0.0,0.0);

void setup() {
  ptime = micros();
  clocktime = ptime;
  
  Sensors::init(0);
  Comm::init(57600);
  
  delay(1000);
}

double pidp, pidy, pidr, chl;

void loop() {
  //Time management
  ctime = micros();
  dt = (ctime-ptime)/1000000.0;
  if(ctime-clocktime >= 1000000){
    Comm::sendTPS(tps);
    Comm::print("mpuSamples ");
    Comm::print(String(Sensors::getSampleCount()));
    Comm::print("\n");
    Sensors::resetSampleCount();
    tps = 0;
    clocktime = ctime;
  }
  tps++;

  if(false && (tps % 100) == 0 ){
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
   pidp = pidPitch.compute(Sensors::getPitch(), Comm::getChPitch(), dt);
   pidy = pidYaw.compute(Sensors::getYaw(), Comm::getChYaw(), dt);
   pidr = pidRoll.compute(Sensors::getRoll(), Comm::getChRoll(), dt);
   chl = Comm::getChLift();

  ptime = ctime;
}

