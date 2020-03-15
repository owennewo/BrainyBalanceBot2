#ifndef MOTOR_H
#define MOTOR_H

void MotorsSetup();

void MotorsEnable();
void MotorsDisable();

void MotorsSetRightSpeed(long stepsPerSecond);
void MotorsSetLeftSpeed(long stepsPerSecond);
float MotorsGetSpeed(float sampleInterval);

#endif
