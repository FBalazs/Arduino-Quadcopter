#ifndef COMM_H
#define COMM_H

#include <Arduino.h>

class CommListener {
public:
  virtual void onControlCommand(const String params[]) = 0;
  virtual void onGainCommand(const String params[]) = 0;
  virtual void onUnkownCommand(const String& cmd, const String params[], int paramLength) = 0;
};

class Comm {
  CommListener& listener;
  String ccmd;
  
  void parseCommand(const String& cmd, const String params[], int pn);
  
public:
  Comm(long baud, CommListener& listener);
  
  void update();
  void send(const String& str);
  void sendCmd(const String& cmd);
  void sendTPS(int tps);
  void sendError();
};

#endif //COMM_H
