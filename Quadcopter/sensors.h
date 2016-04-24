#ifndef SENSORS_H
#define SENSORS_H

namespace Sensors {
  void init(int PIN_INT);
  void update();
  float getPitch();
  float getYaw();
  float getRoll();
  long getSampleCount();
  void resetSampleCount();
}

#endif //SENSOR_H
