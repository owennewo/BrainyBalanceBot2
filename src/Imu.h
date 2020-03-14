#ifndef _IMU_H

#define _IMU_H

void ImuSetup(uint8_t interruptPin);
bool ImuHasData();
float ImuGetAngle();
float ImuGetSampleInterval();
boolean ImuCrashed(float angle);

#endif