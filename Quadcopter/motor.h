#ifndef MOTOR_H
#define MOTOR_H

#include <Servo.h>

class Motor{
private:
  static const int MAX_POW = 250;
  static const int MID_POW = 80;
  static const int MIN_POW = 20;
  int pin;
public:
  Motor(int pin):pin(pin){}
  void updateSpeed(double);
  void calibrate();
};


#endif //MOTOR_H
