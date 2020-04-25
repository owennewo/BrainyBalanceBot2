#ifndef PID_H
#define PID_H

void PidUpdate(float _kp_speed, float _ki_speed, float _kp_angle, float _kd_angle);
float PidSpeedToAngle(long speed, long setSpeed, float frequency, boolean crashed);
long PidAngleToSpeed(float angle, float setAngle, float frequency);

#endif