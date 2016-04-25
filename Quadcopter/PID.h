#ifndef PID_H
#define PID_H

class PID {
  double pGain, iGain, dGain;
  double pState;
  double integral;
  double output;
public:
  
  PID(double p, double i, double d):pGain(p),iGain(i),dGain(d),integral(0),pState(0){}

  double getOutput() {
    return output;
  }
  
  double compute(double state, double setpoint, double dt) {
    double err = setpoint-state;
    output = pGain*err + iGain*integral - dGain*(state-pState);
    integral += err;
    pState = state;
    return output;
  }

  void updateGains(double p, double i, double d) {
    pGain = p;
    iGain = i;
    dGain = d;
  }

  void reset() {
    integral = 0;
    pState = 0;
  }
};

#endif //PID_H
