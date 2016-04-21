#include <Arduino.h>
#include "PID.h"

PID::PID(double p, double i, double d){
  pGain = p;
  iGain = i;
  dGain = d;
  integral = 0;
  pErr = 0;
}
  
double PID::compute(double err){
  double toReturn = pGain*err + iGain*integral + dGain*(err-pErr);
  integral += err;
  pErr = err;
  return toReturn;
}

double PID::updateGains(double p, double i, double d){
  pGain = p;
  iGain = i;
  dGain = d;
}

void PID::reset(){
  integral = 0;
  pErr = 0;
}

