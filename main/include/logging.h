#ifndef LOGGING_H
#define LOGGING_H

#include <SD.h>
extern File dataFile;

void initLogging();
void logToSDCardBuffered(const String& data);
void sensor_logToSDCard();
void push_B_on();
void push_B_off();
float calculateTaskCPULoad(unsigned long executionTime, unsigned long taskPeriod);
float calculateTaskCycleTime(unsigned long &lastTime);
void Core0_PLS();
void Core1_logTask();
void Core1_displayTask();

#endif // LOGGING_H
