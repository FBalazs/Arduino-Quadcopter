#include <MPU6050.h>

MPU6050 mpu6050;

int16_t gyro0_x, gyro0_y, gyro0_z;
int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;

void initMPU(){
  Wire.begin();
  mpu6050.initialize();
  //MPU6050 was placed so that 
  //X is pointing forwards,
  //Y is pointing to the right and
  //Z is pointing down
  //Parameter order was altered so that for us
  //X is pointing to the right
  //Y is pointing upwords
  //Z is pointing backwards
  //All the rotations are clockwise looking at the direction of the corresponding axis
  mpu6050.getMotion6(&accel_z, &accel_x, &accel_y, &gyro0_z, &gyro0_x, &gyro0_y);
  accel_z = -accel_z;
  gyro0_z = -gyro0_z;
  accel_y = -accel_y;
  gyro0_y = -gyro0_y;
}

void readMPU(){
  mpu6050.getMotion6(&accel_z, &accel_x, &accel_y, &gyro_z, &gyro_x, &gyro_y);
  accel_z = -accel_z;
  gyro_z = -gyro_z;
  accel_y = -accel_y;
  gyro_y = -gyro_y;


  //Calculating pitch
  double accelPitch = atan2(accel_y,accel_z) *180/M_PI - 90;
  double gyroPitch = pitch - (long)((gyro_x-gyro0_x)*250/32768*dt); //It is an integral
  //TODO Kalman filter
  while(180 < abs(rawgyro-rawaccel))
    rawaccel += 360*(rawgyro-rawaccel)/abs(rawgyro-rawaccel); //Correction in case it rolled over
  //The accelerometer measure is inaccurate, but it is taken into account
  //so the errors of the gyro integral are corrected
  pitch = rawgyro*0.98+rawaccel*0.02; 
  while(pitch < -180)
    pitch += 360;
  while(180 < pitch)
    pitch -= 360;

    
  //Calculating yaw 
  yaw = (long)(gyro0_y-gyro_y)*250/32768.0;

  //Calculating roll
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



