#ifndef PID_H
#define PID_H

class PID {
public:
  double pGain, iGain, dGain;
  double pErr;
  double integral;
  
  PID(double p, double i, double d);
  
  double compute(double err);

  double updateGains(double p, double i, double d);

  void reset();
  
};

#endif //PID_H
