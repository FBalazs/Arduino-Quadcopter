void initDebug(long baud){
  Serial.begin(baud);
}

void printPYR(double pitch, double yaw, double roll){
  Serial.print(pitch);
  Serial.print("\t");
  Serial.print(yaw);
  Serial.print("\t");
  Serial.println(roll);
}

void debugTPS(int tps){
  Serial.print(tps);
  Serial.println(" TPS");
}

