#include "comm.h"

#include <Arduino.h>

#define xbee Serial

#define PITCH_MAX 30
#define ROLL_MAX 30
#define YAW_MAX 30

namespace Comm {
  double ch_lift = 0, ch_pitch = 0, ch_yaw = 0, ch_roll = 0;
  long lastcontroltime = -1;
  String ccmd = "";

  long getLastControlTime() {
    return lastcontroltime;
  }

  double getChLift() {
    return ch_lift;
  }

  double getChPitch() {
    return ch_pitch;
  }

  double getChYaw() {
    return ch_yaw;
  }

  double getChRoll() {
    return ch_roll;
  }
  
  void init(long baud) {
    xbee.begin(baud);
    while(!xbee);
  }
  
  void parseCommand(String cmd, String params[], int pn) {
    if(cmd == "ch" && pn == 5){
      ch_lift = params[0].toInt()/(double)params[4].toInt();
      ch_pitch = params[1].toInt()/(double)params[4].toInt()*PITCH_MAX;
      ch_yaw = params[2].toInt()/(double)params[4].toInt()*YAW_MAX;
      ch_roll = params[3].toInt()/(double)params[4].toInt()*ROLL_MAX;
      lastcontroltime = millis();
      return;
    }
    if(cmd == "g" && pn == 4){
      //pitchIntegral = 0;
      //rollIntegral = 0;
      //yawIntegral = 0;
      //rollP = pitchP = yawP = params[0].toInt()/(double)atol(params[3].c_str());
      //rollI = pitchI = yawI = params[1].toInt()/(double)atol(params[3].c_str());
      //rollD = pitchD = yawD = params[2].toInt()/(double)atol(params[3].c_str());
      //Serial.print(pitchP*atol(params[3].c_str()));
      //Serial.print('\t');
      //Serial.print(pitchI*atol(params[3].c_str()));
      //Serial.print('\t');
      //Serial.println(pitchD*atol(params[3].c_str()));
      //return;
    }
    sendError();
  }
  
  void update() {
    while(xbee.available()){
      char c = (char)xbee.read();
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

  void print(String str) {
    xbee.print(str);
  }
  
  void sendTPS(int tps){
    xbee.print("tps ");
    xbee.print(tps);
    xbee.print('\n');
  }

  void sendError() {
    xbee.println("error");
  }
}

