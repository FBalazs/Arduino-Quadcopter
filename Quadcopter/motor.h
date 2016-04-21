#ifndef MOTOR_H
#define MOTOR_H

#include <Servo.h>

class Motor{
private:
  Servo mot;
public:
  Motor(int pin){mot.attach(pin); mot.write(0);}
  void updateSpeed(int speed){mot.write(speed);}
};


#endif //MOTOR_H
