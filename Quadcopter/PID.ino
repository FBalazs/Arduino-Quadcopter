/*#include <Arduino.h>


class PID {
public:
  double pGain, iGain, dGain;
  double pErr;
  double integral;
  
  PID(double p, double i, double d){
    pGain = p;
    iGain = i;
    dGain = d;
    integral = 0;
    pErr = 0;
  }
  
  double compute(double err){
    double toReturn = pGain*err + iGain*integral + dGain*(err-pErr);
    integral += err;
    pErr = err;
    return toReturn;
  }

  double updateGains(double p, double i, double d){
    pGain = p;
    iGain = i;
    dGain = d;
  }

  void reset(){
    integral = 0;
    pErr = 0;
  }
  
};*/

