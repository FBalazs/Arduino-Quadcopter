MPU6050 mpu6050;


int16_t gyro0_x, gyro0_y, gyro0_z;
int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;

void initMPU(){
  Wire.begin();
  mpu6050.initialize();
  //MPU6050 was place so that 
  //X is pointing forwards,
  //Y is pointing to the right and
  //Z is pointing down
  //Parameter order was altered so that for us
  //X is pointing to the right
  //Y is pointing upwords
  //Z is pointing bachwards
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
  
}



