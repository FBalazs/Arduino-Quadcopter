MPU6050 mpu6050;


int16_t gyro0_x, gyro0_y, gyro0_z;
int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;

void initMPU(){
  Wire.begin();
  mpu.initialize();
  mpu.getMotion6(&accel_z, &accel_x, &accel_y, &gyro0_z, &gyro0_x, &gyro0_y);
  accel_z = -accel_z;
  gyro0_z = -gyro0_z;
  accel_y = -accel_y;
  gyro0_y = -gyro0_y;
  //TODO double check directions
}

void readMPU(){
  mpu.getMotion6(&accel_z, &accel_x, &accel_y, &gyro_z, &gyro_x, &gyro_y);
  accel_z = -accel_z;
  gyro_z = -gyro_z;
  accel_y = -accel_y;
  gyro_y = -gyro_y;
  //TODO double check directions
  //TODO Calculate pii
}



