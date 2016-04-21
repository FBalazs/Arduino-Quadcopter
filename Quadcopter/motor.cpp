#include "motor.h"

#include <Arduino.h>

void Motor::calibrate(){
  delay(1500);
  servo.writeMicroseconds(MAX_PULSE_LENGTH);
  delay(500);
  servo.writeMicroseconds(MID_PULSE_LENGTH);
  delay(1200);
  servo.writeMicroseconds(MIN_PULSE_LENGTH);
  delay(2000);
}

void Motor::updateSpeed(double speed){
  if(speed > 1.0)
    speed = 1.0;
  if(speed < 0.0)
    speed = 0.0;
  servo.writeMicroseconds(speed*(MAX_PULSE_LENGTH - MIN_PULSE_LENGTH) + MIN_PULSE_LENGTH);
}

