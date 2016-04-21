#ifndef SENSORS_H
#define SENSORS_H

namespace Sensors {
  void init(int PIN_INT);
  void update();
  double getPitch();
  double getYaw();
  double getRoll();
}

#endif
