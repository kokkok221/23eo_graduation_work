#ifndef SENSOR_H
#define SENSOR_H

#include <Wire.h>
#include <Arduino.h>


// MPU6886 definitions
#define MPU6886_ADDR 0x68
#define MPU6886_REG_CONF    0x1A
// AS5601 definitions
#define AS5601_ADDR 0x36
#define AS5601_REG_ABN 0x09
#define AS5601_REG_CONF 0x07
#define AS5601_RAW_ANGLE 0x0C


//extern volatile int32_t leftEncoderCount;
//extern volatile int32_t rightEncoderCount;

//mpu6886 init & LPF setting
void initMPU6886(uint8_t dlpfSetting, int maxRetries = 3);
void get_mpu6886();

//AS5601 init & resolution/LPF setting
bool init_AS5601(TwoWire &i2cBus, uint8_t deviceAddress, uint8_t registerAddress, uint8_t value, int maxRetries = 3);
void setAS5601Filter(TwoWire &i2cBus, uint8_t slowFilter, uint8_t fastFilterThreshold, int maxRetries = 3);
void AS5601_CounterInit();
void get_AS5601();
void debug_AS5601(TwoWire &i2cBus, const char* encoderName);
void debug_MPU6886Filter();
void scanI2C(TwoWire &i2cBus, const char* busName);


#endif // SENSOR_H
