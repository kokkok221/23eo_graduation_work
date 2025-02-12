#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

// mpu6886の設定値
extern const uint8_t dlpfSetting;

// ファイル名
extern const char* LOG_FILENAME;

// ピン番号
extern const int MOTOR_CW;
extern const int MOTOR_CCW;
extern const int CW_CHANNEL;
extern const int CCW_CHANNEL;
extern const int MOTOR_PLT_LAMP;
extern const int PLT_LAMP_CHANNEL;

// PID制御の初期パラメータ
extern const float P_P;
extern const float P_I;
extern const float P_D;
extern const float TARGET;
extern const float W_P;
extern const float W_I;
extern const float W_D;
extern const float ANTI_WINDUP_GAIN;
extern const int MOTOR_OFFSET;

// その他の定数
constexpr int BUFFER_SIZE = 128;

#endif // CONSTANTS_H
