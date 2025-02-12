#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

/*
//left Motor
#define L_MOTOR_CW 27
#define L_MOTOR_CCW 19
#define L_CW_CHANNEL 0
#define L_CCW_CHANNEL 1

 //right Motor
#define R_MOTOR_CW 13
#define R_MOTOR_CCW 14
#define R_CW_CHANNEL 3
#define R_CCW_CHANNEL 4
*/


void initMotor();
void PWM_pd();
void PWM_pid();
void sensor_disturbance1();
void sensor_disturbance2();

#endif // MOTOR_H
