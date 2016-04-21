#ifndef MOTOR_H
#define MOTOR_H

#include <Servo.h>

class Motor{
private:
  static const int MIN_PULSE_LENGTH = 1150;
  static const int MID_PULSE_LENGTH = 1500;
  static const int MAX_PULSE_LENGTH = 1950;
  Servo servo;
public:
  Motor(int pin){servo.attach(pin);}
  void updateSpeed(double);
  void calibrate();
};


#endif //MOTOR_H
