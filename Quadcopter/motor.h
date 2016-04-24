#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor{
private:
  static const int MAX_POW = 250;
  static const int MID_POW = 150;
  static const int MIN_POW = 100;
  int pin;
  
public:
  Motor(int pin):pin(pin) {
    pinMode(pin, OUTPUT);
  }
  void updateSpeed(double);
  void calibrate();
};


#endif //MOTOR_H
