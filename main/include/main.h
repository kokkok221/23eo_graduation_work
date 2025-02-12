#ifndef MAIN_H
#define MAIN_H

#include <M5Core2.h>
#include <Wire.h>

//display values
extern int button_Param;
extern int log_state;

//mpu6886 values
extern uint8_t configValue;
extern float gyroX, gyroY, gyroZ;
extern float pitch, roll, yaw;

// PID
extern float p_p, p_i, p_d, target;
extern float P, I, D, prepP;
extern float error, power, clamp;
extern float antiWindupGain;

// PWM
extern float dt, power;
extern unsigned long preTime;
extern int Motor_offset;
extern int Duty;
extern bool motorOutputEnabled;

//log variables
//#define BUFFER_SIZE 10
extern float cycleTimeCore0 ;       // サイクル時間（秒）
extern unsigned long lastCycleTimeCore0; // 前回のサイクル時間記録（ミリ秒）
extern float cycleTimeCore1_1;       // サイクル時間（秒）
extern unsigned long lastCycleTimeCore1_1; // 前回のサイクル時間記録（ミリ秒）
extern float cycleTimeCore1_2;       // サイクル時間（秒）
extern unsigned long lastCycleTimeCore1_2; // 前回のサイクル時間記録（ミリ秒）
extern float core0TaskLoadBuffer[];
extern float core0CycleTimeBuffer[];
extern float core1_1TaskLoadBuffer[];
extern float core1_1CycleTimeBuffer[];
extern float core1_2TaskLoadBuffer[];
extern float core1_2CycleTimeBuffer[];
extern int core0BufferIndex;
extern int core1_1BufferIndex;
extern int core1_2BufferIndex;


// 関数プロトタイプの宣言
void setup();
void loop();

#endif // MAIN_H
