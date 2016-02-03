#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#define xbee_RX 3 //Rx of the xbee Tx of the Arduino
#define xbee_TX 2 //Rx of the xbee Tx of the Arduino
#define xbee_BAUD 57600

#define LEFT_HOR A0
#define LEFT_VER A1
#define RIGHT_HOR A2
#define RIGHT_VER A3
#define LEFT_BTN 7
#define RIGHT_BTN 6

SoftwareSerial xbee(xbee_TX,xbee_RX);
LiquidCrystal lcd(9,8,13,12,11,10);

String command = "";

long lastCommand = 0;

void receive(){
  char received = 'a';
  while(received != -1){
    received = xbee.read();
    command += received;
    if(received == '\n'){
      parseCommand();
      command = "";
      lastCommand = millis();
    }
  }
}

void lcdPrint(String toPrint){
  
}

void parseCommand(){
  String cmd;
  int pn;
  int i = command.indexOf('\t');
  if(i != -1){//Yes parameters
    cmd = command.substring(0,i);
    command = command.substring(i+1);
    pn = 1;
    for(i = command.indexOf('\t'); i!=-1; i = command.indexOf('\t',i+1)){
      pn++;
    }
  }
  else {
    pn = 0;//No parameters
    cmd = command.substring(0,command.length()-1);
  }
  String params[pn];
  for(i= 0; i<pn; ++i){
    int j = command.indexOf('\t');
    if(j == -1) j = command.length();
    params[i] = command.substring(0,j);
    command = command.substring(j+1);
  }

  if(cmd == "print" && pn == 1){
    lcdPrint(params[0]);
  }
}

void setup() {
  xbee.begin(xbee_BAUD);
  Serial.begin(115200);
  
}

void sendControls(){
  int ch_lift = analogRead(LEFT_VER);
  int ch_pitch = analogRead(RIGHT_VER);
  int ch_yaw = analogRead(LEFT_HOR);
  int ch_roll = analogRead(RIGHT_HOR);
  xbee.print("ch\t");
  xbee.print(ch_lift);
  xbee.print("\t");
  xbee.print(ch_pitch);
  xbee.print("\t");
  xbee.print(ch_yaw);
  xbee.print("\t");
  xbee.print(ch_roll);
  xbee.print("\t");
  xbee.print(1024);
  xbee.print("\n");
  
  Serial.print("ch\t");
  Serial.print(ch_lift);
  Serial.print("\t");
  Serial.print(ch_pitch);
  Serial.print("\t");
  Serial.print(ch_yaw);
  Serial.print("\t");
  Serial.print(ch_roll);
  Serial.print("\t");
  Serial.print(1024);
  Serial.print("\n");
}

void loop() {
  if(xbee.available())
    receive();
  sendControlls();
}
