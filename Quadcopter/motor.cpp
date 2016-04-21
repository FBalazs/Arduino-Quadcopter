#include "motor.h"

#include <Arduino.h>

void Motor::calibrate(){
  delay(1500);
  analogWrite(pin,MAX_POW);
  delay(500);
  analogWrite(pin,MID_POW);
  delay(1200);
  analogWrite(pin,MIN_POW);
  delay(2000);
}


void Motor::updateSpeed(double speed){
  if(speed > 1.0)
    speed = 1.0;
  if(speed < 0.0)
    speed = 0.0;
  analogWrite(pin,speed*(MAX_POW-MIN_POW) + MIN_POW);
}

