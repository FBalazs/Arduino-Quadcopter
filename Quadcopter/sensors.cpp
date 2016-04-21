#include <Arduino.h>

#include <MPU6050.h>

namespace Sensors {
  MPU6050 mpu;
  double pitch, yaw, roll;

  double getPitch() {
    return pitch;
  }

  double getYaw() {
    return yaw;
  }

  double getRoll() {
    return roll;
  }
  
  void init() {
    Wire.begin();
    mpu.initialize();
    
  }

  void update() {

  }
}
