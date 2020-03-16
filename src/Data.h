#include <Arduino.h>
#include <User_Defines.h>

#ifndef _DATA_H
#define _DATA_H

struct BotState {
    long steeringCommand; // this is the steering wanted by user
    long speedCommand; // this is the current speed wanted by user
    long speed; // this is the actual/estimated speed
    long speedTarget;  // this is the target speed - to go forwards we need to start going backwards!
    float angle; // this is the actual/estimated angle
    float angleTarget; // this is the target angle
    bool crashed;
    float sampleInterval;
    float lastAngle;
    float angularSpeed;
    long lastSampleTime;

    void setAngle(float _angle) {
        angle = _angle;
        long now = micros();
        sampleInterval = (float) (now - lastSampleTime) / 1000000; 
        lastSampleTime = now; 
        angularSpeed = (angle - lastAngle) / sampleInterval;
        lastAngle = angle;
        crashed = (abs(angle) > CRASH_ANGLE);
    }
};

#endif