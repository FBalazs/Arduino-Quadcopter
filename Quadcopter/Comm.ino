#include <Arduino.h>

#define xbee Serial1
#define BAUD 57600

#define PITCH_MAX 30
#define ROLL_MAX 30
#define YAW_MAX 30


/*void initComm() {
  xbee.begin(BAUD);
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
    pitchIntegral = 0;
    rollIntegral = 0;
    yawIntegral = 0;
    rollP = pitchP = yawP = params[0].toInt()/(double)atol(params[3].c_str());
    rollI = pitchI = yawI = params[1].toInt()/(double)atol(params[3].c_str());
    rollD = pitchD = yawD = params[2].toInt()/(double)atol(params[3].c_str());
    Serial.print(pitchP*atol(params[3].c_str()));
    Serial.print('\t');
    Serial.print(pitchI*atol(params[3].c_str()));
    Serial.print('\t');
    Serial.println(pitchD*atol(params[3].c_str()));
    return;
  }
  xbee.println("error");
}

void updateComm() {
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
}*/

