#include "comm.h"

#include <Arduino.h>

#define CommSerial Serial

Comm::Comm(long baud, CommListener& listener):listener(listener) {
  CommSerial.begin(baud);
  while(!CommSerial);
}
  
void Comm::parseCommand(const String& cmd, const String params[], int pn) {
  if(cmd == "ch" && pn == 4){
    listener.onControlCommand(params);
    return;
  }
  if(cmd == "g" && pn == 3){
    listener.onGainCommand(params);
    return;
  }
  sendError();
}

void Comm::update() {
  while(CommSerial.available()){
    char c = (char)CommSerial.read();
    ccmd += c;
    if(c == '\n'){
      String cmd = "";
      int pn = 0;
      for(int i = 0; i < ccmd.length(); i++){
        if(ccmd[i] == '\t' || ccmd[i] == ' ')
          pn++;
        if(pn == 0)
          cmd += ccmd[i];
      }
      String params[pn];
      int i = 0;
      String cparam = "";
      for(int j = cmd.length()+1; j < ccmd.length(); j++)
        if(ccmd[j] == '\t' || ccmd[j] == ' ' || ccmd[j] == '\n'){
          params[i] = cparam;
          cparam = "";
          i++;
        }
        else
          cparam += ccmd[j];
      parseCommand(cmd, params, pn);
      ccmd = "";
    }
  }
}

void Comm::send(const String& str) {
  CommSerial.print(str);
  CommSerial.flush();
}

void Comm::sendCmd(const String& cmd) {
  CommSerial.println(cmd);
  CommSerial.flush();
}

void Comm::sendTPS(int tps){
  sendCmd(String("tps: ")+tps);
}

void Comm::sendError() {
  sendCmd(String("error"));
}

