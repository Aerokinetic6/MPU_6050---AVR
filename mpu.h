#define MPU_ADDR        0x68


////////--- REGS ---//////////

#define SMPLRT_DIV      0x19

#define CONFIG          0x1A
#define GYRO_CONFIG     0x1B

#define INT_ENABLE      0x38


// sensor regs //

#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40

#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42

#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48

#define PWR_MGMT_1      0x6B


////////--- OFFSETS ----///////

#define ACC_X_OFFSET    -850
#define ACC_Y_OFFSET    -650
#define ACC_Z_OFFSET    4266

#define GYRO_X_OFFSET   -70
#define GYRO_Y_OFFSET   -30
#define GYRO_Z_OFFSET   10

