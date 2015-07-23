#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Servo.h"
#include "SPI.h"
#include "SD.h"

#define xbee Serial1
#define BAUD 57600

#define PITCH_MAX 30
#define ROLL_MAX 30
#define YAW_MAX 30

#define MOTOR_PIN_FL 9
#define MOTOR_PIN_FR 10
#define MOTOR_PIN_BL 11
#define MOTOR_PIN_BR 12
#define MOTOR_MIN 50
#define MOTOR_MAX 180

#define BUTTON_PIN 8
#define LED_PIN 7

#define SD_PIN 53

Servo motorFL, motorFR, motorBL, motorBR;
MPU6050 mpu;
String ccmd;
int16_t gyro0_x, gyro0_y, gyro0_z;
int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;
double pitch = 0, yaw = 0, roll = 0;
double ch_lift = 0, ch_pitch = 0, ch_yaw = 0, ch_roll = 0;

double out_pitch = 0, out_yaw = 0, out_roll = 0;
double pitchP = 0, pitchI = 0, pitchD = 0, pitchO;
double yawP = 0, yawI = 0, yawD = 0, yawO;
double rollP = 0, rollI = 0, rollD = 0, rollO;
double pitchIntegral = 0, yawIntegral = 0, rollIntegral = 0;
double pErrPitch = 0, pErrRoll = 0, pErrYaw = 0;

int tps;
double dt;
long ctime, ptime, clocktime, lastcontroltime = -1;


int mincps = 25; // command per second

boolean printPWM = false, printPosition = false, printPID = false;
int printCounter = 0;

boolean logSD = true;
File logFile;
int logNumber = 0;
char fileName[20];

void setup() {
  pinMode(53,OUTPUT);
  Serial.begin(BAUD);
  if(logSD) {
    pinMode(SD_PIN, OUTPUT);
    if(!SD.begin(SD_PIN)) {
      Serial.println("error sd.begin");
      logSD = false;
    }
   String(String(logNumber,DEC)+"datalog.txt").toCharArray(fileName,20);
   while(SD.exists(fileName)){
     logNumber++;
     String(String(logNumber,DEC)+"datalog.txt").toCharArray(fileName,20);
   }
   Serial.println(String(String(logNumber,DEC)+"datalog.txt"));
   logFile = SD.open(fileName, O_CREAT | O_WRITE);
    if(logFile) {
        Serial.print("created file: ");
        Serial.println(fileName);
        logFile.println("ptime;ctime;pitch;yaw;roll;ch_pitch;ch_yaw;ch_roll;out_pitch;out_yaw;out_roll;pitchIntegral;yawIntegral;rollIntegral");
        logFile.close();
    }
    else  {
      Serial.println("error sd.open");
      logSD = false;
    }
  }
  xbee.begin(BAUD);
  xbee.println("hello");
  Wire.begin();
  mpu.initialize();
  mpu.getMotion6(&accel_z, &accel_x, &accel_y, &gyro0_z, &gyro0_x, &gyro0_y);
  accel_z = -accel_z;
  gyro0_z = -gyro0_z;
  accel_y = -accel_y;
  gyro0_y = -gyro0_y;
  ccmd = "";
  
  /*pinMode(MOTOR_PIN_FL, OUTPUT);
  pinMode(MOTOR_PIN_FR, OUTPUT);
  pinMode(MOTOR_PIN_BL, OUTPUT);
  pinMode(MOTOR_PIN_BR, OUTPUT);*/
  motorFL.attach(MOTOR_PIN_FL);
  motorFL.write(0);
  motorFR.attach(MOTOR_PIN_FR);
  motorFR.write(0);
  motorBL.attach(MOTOR_PIN_BL);
  motorBL.write(0);
  motorBR.attach(MOTOR_PIN_BR);
  motorBR.write(0);
  
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  xbee.println("start");
  //Serial.begin(BAUD);
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

void readCommands() {
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

void readMPU() {
  mpu.getMotion6(&accel_z, &accel_x, &accel_y, &gyro_z, &gyro_x, &gyro_y);
  accel_z = -accel_z;
  gyro_z = -gyro_z;
  accel_y = -accel_y;
  gyro_y = -gyro_y;
  
  double rawaccel = -(atan2(accel_y, accel_z)*180/M_PI - 90);
  double rawgyro = pitch-((long)(gyro_x-gyro0_x)*250/32768*dt);
  while(180 < abs(rawgyro-rawaccel))
    rawaccel += 360*(rawgyro-rawaccel)/abs(rawgyro-rawaccel);
  pitch = rawgyro*0.98+rawaccel*0.02;
  while(pitch < -180)
    pitch += 360;
  while(180 < pitch)
    pitch -= 360;
  
  yaw = (long)(gyro0_y-gyro_y)*250/32768.0;
  
  rawaccel = atan2(accel_y, accel_x)*180/M_PI - 90;
  rawgyro = roll-((long)(gyro_z-gyro0_z)*250/32768*dt);
  while(180 < abs(rawgyro-rawaccel))
    rawaccel += 360*(rawgyro-rawaccel)/abs(rawgyro-rawaccel);
  roll = rawgyro*0.98+rawaccel*0.02;
  while(roll < -180)
    roll += 360;
  while(180 < roll)
    roll -= 360;
}

void computePID() {
  double err;
  if(abs(ch_pitch-pitch) < abs(360-(ch_pitch-pitch)))
    err = ch_pitch-pitch;
  else
    err = 360-(ch_pitch-pitch);
  out_pitch = pitchP*err + pitchI*pitchIntegral + pitchD*(err-pErrPitch)/dt;
  pitchIntegral += err*dt;
  pErrPitch = err;
  out_pitch = 0;
  
  err = ch_yaw-yaw;
  out_yaw = yawP*err + yawI*yawIntegral + yawD*(err-pErrYaw)/dt;
  yawIntegral += err*dt;
  pErrYaw = err;
  out_yaw = 0;
  
  if(abs(ch_roll-roll) < abs(360-(ch_roll-roll)))
    err = ch_roll-roll;
  else
    err = 360-(ch_roll-roll);
  out_roll = rollP*err + rollI*rollIntegral + rollD*(err-pErrRoll)/dt;
  rollIntegral += err*dt;
  pErrRoll = err;
}

void updateMotors() {
  int pwr = MOTOR_MIN + (MOTOR_MAX-MOTOR_MIN)*(ch_lift - out_pitch - out_yaw + out_roll);
  if(pwr < MOTOR_MIN)
    pwr = MOTOR_MIN;
  if(pwr > MOTOR_MAX)
    pwr = MOTOR_MAX;
  if(ch_lift == 0)
    pwr = MOTOR_MIN;
  //analogWrite(MOTOR_PIN_FL, pwr);
  motorFL.write(pwr);
  if(printPWM && printCounter == 0) {
    Serial.print(pwr);
    Serial.print("\t");
  }
  
  pwr = MOTOR_MIN + (MOTOR_MAX-MOTOR_MIN)*(ch_lift - out_pitch + out_yaw - out_roll);
  if(pwr < MOTOR_MIN)
    pwr = MOTOR_MIN;
  if(pwr > MOTOR_MAX)
    pwr = MOTOR_MAX;
  if(ch_lift == 0)
    pwr = MOTOR_MIN;
  //analogWrite(MOTOR_PIN_FR, pwr);
  motorFR.write(pwr);
  if(printPWM && printCounter == 0) {
    Serial.print(pwr);
    Serial.print("\t");
  }
  
  pwr = MOTOR_MIN + (MOTOR_MAX-MOTOR_MIN)*(ch_lift + out_pitch + out_yaw + out_roll);
  if(pwr < MOTOR_MIN)
    pwr = MOTOR_MIN;
  if(pwr > MOTOR_MAX)
    pwr = MOTOR_MAX;
  if(ch_lift == 0)
    pwr = MOTOR_MIN;
  //analogWrite(MOTOR_PIN_BL, pwr);
  motorBL.write(pwr);
  if(printPWM && printCounter == 0) {
    Serial.print(pwr);
    Serial.print("\t");
  }
  
  pwr = MOTOR_MIN + (MOTOR_MAX-MOTOR_MIN)*(ch_lift + out_pitch - out_yaw - out_roll);
  if(pwr < MOTOR_MIN)
    pwr = MOTOR_MIN;
  if(pwr > MOTOR_MAX)
    pwr = MOTOR_MAX;
  if(ch_lift == 0)
    pwr = MOTOR_MIN;
  //analogWrite(MOTOR_PIN_BR, pwr);
  motorBR.write(pwr);
  if(printPWM && printCounter == 0) {
    Serial.println(pwr);
  }
  
  printCounter++;
  if(10 <= printCounter)
    printCounter = 0;
}

void loop() {
  //Serial.println("loop()");
  ctime = micros();
  dt = (ctime-ptime)/1000000.0;
  if(ctime-clocktime >= 1000000){
    xbee.print("print tps ");
    xbee.print(tps);
    xbee.print('\n');
    Serial.print("tps ");
    Serial.println(tps);
    tps = 0;
    clocktime = ctime;
  }
  tps++;
  
  //Serial.println("readCommands()");
  readCommands();
  if(millis()-lastcontroltime >= 1000/mincps){
    ch_lift = 0;
    if(printCounter == 0) {
      Serial.print("No signal for: ");
      Serial.print(millis()-lastcontroltime);
      Serial.println(" ms!");
    }
  }
  
  //Serial.println("readMPU()");
  readMPU();
  if(printPosition && printCounter == 0) {
    Serial.print(pitch);
    Serial.print(" -> ");
    Serial.print(ch_pitch);
    Serial.print("\t");
    Serial.print(yaw);
    Serial.print(" -> ");
    Serial.print(ch_yaw);
    Serial.print("\t");
    Serial.print(roll);
    Serial.print(" -> ");
    Serial.println(ch_roll);
  }
  
  //Serial.println("computePID()");
  computePID();
  if(printPID && printCounter == 0) {
    Serial.print(out_pitch*100);
    Serial.print("\t");
    Serial.print(out_yaw*100);
    Serial.print("\t");
    Serial.println(out_roll*100);
  }
  
  //Serial.println("updateMotors()");
  updateMotors();
  
  if(logSD) {
    logFile = SD.open(fileName, O_APPEND | O_WRITE);
    if(logFile){
      logFile.print(ptime);
      logFile.print(';');
      logFile.print(ctime);
      logFile.print(';');
      logFile.print((int)(pitch*1000));
      logFile.print(';');
      logFile.print((int)(yaw*1000));
      logFile.print(';');
      logFile.print((int)(roll*1000));
      logFile.print(';');
      logFile.print((int)(ch_pitch*1000));
      logFile.print(';');
      logFile.print((int)(ch_yaw*1000));
      logFile.print(';');
      logFile.print((int)(ch_roll*1000));
      logFile.print(';');
      logFile.print((int)(out_pitch*1000));
      logFile.print(';');
      logFile.print((int)(out_yaw*1000));
      logFile.print(';');
      logFile.print((int)(out_roll*1000));
      logFile.print(';');
      logFile.print(pitchIntegral);
      logFile.print(';');
      logFile.print(yawIntegral);
      logFile.print(';');
      logFile.print(rollIntegral);
      logFile.println();
    }
    logFile.close();
  }
  
  ptime = ctime;
  
  //Serial.println("end");
}
