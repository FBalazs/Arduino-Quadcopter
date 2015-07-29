#include <Arduino.h>


void initDebug(int baud){
  Serial.begin(baud);
}

void printPYR(double pitch, double yaw, double roll){
  Serial.print(pitch);
  Serial.print("\t");
  Serial.print(yaw);
  Serial.print("\t");
  Serial.print(roll);
  Serial.print("\t");
}

