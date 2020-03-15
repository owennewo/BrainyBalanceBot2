#ifndef _IMU_H

#define _IMU_H

void ImuSetup();
bool ImuHasData();
float ImuGetAngle();
float ImuGetSampleInterval();
bool ImuCrashed(float angle);

#endif