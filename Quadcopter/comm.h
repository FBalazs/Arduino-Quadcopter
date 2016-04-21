#ifndef COMM_H
#define COMM_H

#include <Arduino.h>

namespace Comm {
  void init(long baud);
  void update();
  void print(String str);
  void sendTPS(int tps);
  void sendError();
  long getLastControlTime();
  double getChLift();
  double getChPitch();
  double getChYaw();
  double getChRoll();
}

#endif //COMM_H
