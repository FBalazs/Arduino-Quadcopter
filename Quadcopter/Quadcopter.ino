#include "sensors.h"
#include "comm.h"
#include "motor.h"
#include "PID.h"

class QuadControl : public CommListener {
  int tps; //Ticks per second
  long ctime; //Time of the beginning of the current tick
  long ptime; //Time of the beginning of the previous tick
  double dt; //Time since the last tick in seconds
  long clocktime, statetime; //Used to count TPS, stores the time since the last TPS print
  
  Comm comm;
  PID pidPitch, pidYaw, pidRoll;
  Motor motFL, motFR, motBL, motBR;

  float chl, chp, chy, chr;
  double pidp, pidy, pidr;

public:
  QuadControl():tps(0), clocktime(0),
                comm(57600, *this),
                pidPitch(0.25, 0.0, 0.0),
                pidYaw(0.0, 0.0, 0.0),
                pidRoll(0.25, 0.0, 0.0),
                motFL(9),
                motFR(10),
                motBL(11),
                motBR(12) {
    ptime = micros();
    clocktime = ptime;
    statetime = ptime;

    Sensors::init(0);
    motFL.updateSpeed(0.0);
    motFR.updateSpeed(0.0);
    motBL.updateSpeed(0.0);
    motBR.updateSpeed(0.0);
    
    //delay(1000);
  }

  void onControlCommand(const String params[]) {
    chl = params[0].toFloat();
    chp = params[1].toFloat();
    chy = params[2].toFloat();
    chr = params[3].toFloat();
  }
  
  void onGainCommand(const String params[]) {
    float gp = params[0].toFloat();
    float gi = params[1].toFloat();
    float gd = params[2].toFloat();
    pidPitch.updateGains(gp, gi, gd);
    pidYaw.updateGains(gp, gi, gd);
    pidRoll.updateGains(gp, gi, gd);
    comm.send(String("hello\n"));
  }
  
  void onUnkownCommand(const String& cmd, const String params[], int paramLength) {
    comm.sendError();
  }

  double mfl, mfr, mbl, mbr;

  void update() {
    //Time management
    ctime = micros();
    dt = (ctime-ptime)/1000000.0;
    if(ctime-clocktime >= 1000000){
      comm.sendTPS(tps);
      comm.send(String("mpuSamples ")+Sensors::getSampleCount()+'\n');
      Sensors::resetSampleCount();
      tps = 0;
      clocktime = ctime;
    }
    tps++;
    
    Sensors::update();
    comm.update();
    
    pidp = pidPitch.compute(Sensors::getPitch(), chp, dt);
    pidy = pidYaw.compute(Sensors::getYaw(), chy, dt);
    pidr = pidRoll.compute(Sensors::getRoll(), chr, dt);
    // DEBUG
    pidy = pidr = 0;
    if(chl < 0.1) {
      motFL.updateSpeed(mfl = 0.0);
      motFR.updateSpeed(mfr = 0.0);
      motBL.updateSpeed(mbl = 0.0);
      motBR.updateSpeed(mbr = 0.0);
    } else {
      motFL.updateSpeed(mfl = chl-pidp+pidr);
      motFR.updateSpeed(mfr = chl-pidp-pidr);
      motBL.updateSpeed(mbl = chl+pidp+pidr);
      motBR.updateSpeed(mbr = chl+pidp-pidr);
    }

    if(ctime-statetime >= 1000000/25) {
      comm.send("state ");
      comm.send(String(Sensors::getPitch()));
      comm.send(" ");
      comm.send(String(Sensors::getYaw()));
      comm.send(" ");
      comm.send(String(Sensors::getRoll()));
      comm.send(" ");
      comm.send(String(mfl));
      comm.send(" ");
      comm.send(String(mfr));
      comm.send(" ");
      comm.send(String(mbl));
      comm.send(" ");
      comm.send(String(mbr));
      comm.send("\n");
      statetime = ctime;
    }
  
    ptime = ctime;
  }
};

QuadControl* quadControl;

void setup() {
  quadControl = new QuadControl();
}

void loop() {
  quadControl->update();
}

