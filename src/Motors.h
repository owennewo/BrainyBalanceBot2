#ifndef MOTOR_H

#define MOTOR_H

void MotorsSetup(uint8_t leftDirPin, uint8_t leftStepPin, uint8_t rightDirPin, uint8_t rightStepPin, uint8_t enablePin);

void MotorsEnable();
void MotorsDisable();

void MotorsSetRightSpeed(long stepsPerSecond);
void MotorsSetLeftSpeed(long stepsPerSecond);
float MotorsGetSpeed();

#endif
