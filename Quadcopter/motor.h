#ifndef MOTOR_H
#define MOTOR_H

#include <Servo.h>

class Motor{
private:
  static const int MIN_PULSE_LENGTH = 1000;
  static const int MID_PULSE_LENGTH = 1600;
  static const int MAX_PULSE_LENGTH = 2000;
  Servo servo;
  
  static const int MAX_POW = 250;
  static const int MID_POW = 150;
  static const int MIN_POW = 100;
  int pin;
  
public:
  Motor(int pin){servo.attach(pin);}
  void updateSpeed(double);
  void calibrate();
};


#endif //MOTOR_H
