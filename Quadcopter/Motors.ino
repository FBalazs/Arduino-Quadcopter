#include <Arduino.h>

#include <Servo.h>

#define MOTOR_PIN_FL 9
#define MOTOR_PIN_FR 10
#define MOTOR_PIN_BL 11
#define MOTOR_PIN_BR 12

Servo motorFL, motorFR, motorBL, motorBR;

void initMotors(){
  motorFL.attach(MOTOR_PIN_FL);
  motorFR.attach(MOTOR_PIN_FR);
  motorBL.attach(MOTOR_PIN_BL);
  motorBR.attach(MOTOR_PIN_BR);
  motorFL.write(0);
  motorFR.write(0);
  motorBL.write(0);
  motorBR.write(0);
}

void updateMotors(int pwFL, int pwFR, int pwBL, int pwBR){
  motorFL.write(pwFL);
  motorFR.write(pwFR);
  motorBL.write(pwBL);
  motorBR.write(pwBR);
}

