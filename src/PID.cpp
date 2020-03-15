#include <Arduino.h>
#include <User_Defines.h>

float speedIntError = 0;
float lastAngleError = 0;

float kp_speed, ki_speed, kp_angle, kd_angle;

void PidUpdate(float _kp_speed, float _ki_speed, float _kp_angle, float _kd_angle) {
  kp_speed = _kp_speed;
  ki_speed = _ki_speed;
  kp_angle = _kp_angle;
  kd_angle = _kd_angle;
}

float PidSpeedToAngle(long speed, long setSpeed, float sampleInterval, boolean crashed) {

  // float error;
  // float output;
  static float errorSum;

  long speedError = setSpeed - speed;
  errorSum += constrain(speedError, -ITERM_MAX_ERROR, ITERM_MAX_ERROR);
  errorSum = constrain(errorSum, -ITERM_MAX, ITERM_MAX);

  if (crashed) {
    errorSum = 0;  // avoiding integral windup
  }
  

  float angleTarget = kp_speed * speedError + ki_speed * errorSum * sampleInterval;
  
  if (angleTarget > 40) {
    Serial.printf("\n\nBANG! %f %f\n", angleTarget, sampleInterval);
    while(true) ;
  }

  return (angleTarget);
}


float smoothedDeriveError = 0;

long PidAngleToSteps(float angleMeasured, float angleTarget, float sampleInterval) {

  static long angleTargetOld;
  static long angleMeasuredOld;
  long angleError = angleTarget - angleMeasured;
  
  // Kd is implemented in two parts
  //    The biggest one using only the input (sensor) part not the SetPoint input-input(t-1).
  //    And the second using the setpoint to make it a bit more agressive   setPoint-setPoint(t-1)
  float angleTargetDiff = constrain((angleTarget - angleTargetOld), -8, 8); // We limit the input part...
  float angleMeasuredDiff = angleMeasured - angleMeasuredOld;
  float steps = kp_angle * angleError + (kd_angle * (angleTargetDiff - angleMeasuredDiff) / sampleInterval);
  
  angleMeasuredOld = angleMeasured;  // error for Kd is only the input component
  angleTargetOld = angleTarget;

  return (steps);
  
}

