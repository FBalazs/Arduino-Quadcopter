#include <Arduino.h>
#include "motor.h"

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
  servo.writeMicroseconds(speed*(MAX_PULSE_LENGTH-MIN_PULSE_LENGTH) + MIN_PULSE_LENGTH);
}

