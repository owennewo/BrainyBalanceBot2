#include <Arduino.h>
#include <Display.h>

#include <Button2.h>
#include <Imu.h>
#include <PID.h>
#include <Motors.h>
#include <Remote.h>

#define PIN_BUTTON_1 35
#define PIN_BUTTON_2 0
#define PIN_LEFT_STEP 37
#define PIN_LEFT_DIR 38
#define PIN_RIGHT_STEP 32
#define PIN_RIGHT_DIR 33
#define PIN_ENABLE 39
#define PIN_IMU_INTERRUPT 36

#define MAX_ACCEL 15

Button2 btn1(PIN_BUTTON_1);
Button2 btn2(PIN_BUTTON_2);

float speedCommand = 0.0;
float directionCommand = 0.0;
float speed;

void setup()
{
  Serial.begin(115200);
  PidUpdate(0.0027, 0, 200, 0.5);
  DisplaySetup();
  ImuSetup(PIN_IMU_INTERRUPT);
  MotorsSetup(PIN_LEFT_DIR, PIN_LEFT_STEP, PIN_RIGHT_DIR, PIN_RIGHT_STEP, PIN_ENABLE);
  RemoteSetup();
}

void loop()
{

  RemoteLoop();
  // vTaskDelay(portMAX_DELAY);

  if (ImuHasData())
  {
    // Serial.print("-");
    
    float angle = ImuGetAngle();
    float sampleInterval = ImuGetSampleInterval();

    if (ImuCrashed(angle)) {
        MotorsDisable();
        // Serial.print(".");
      } else {
        MotorsEnable();
        speed = MotorsGetSpeed();
        
        float angleTarget = PidSpeedToAngle(speed, speedCommand, sampleInterval);
        float speedTarget = PidAngleToSteps(angle, angleTarget, sampleInterval);

        speedTarget = constrain(speedTarget, speed - MAX_ACCEL, speed + MAX_ACCEL);
        // data.speedTarget = constrain(data.speedTarget, -MAX_SPEED, MAX_SPEED);

        MotorsSetLeftSpeed(2 * (speedTarget - directionCommand));
        MotorsSetRightSpeed(2 * (speedTarget + directionCommand));
        Serial.printf("a:%.1f at:%.1f s:%.1f  st:%.1f i:%.3f\n", angle, angleTarget, speed, speedTarget, sampleInterval);
      }
  }
  delay(1);
  // Serial.print("+");
  // DisplayLoop();

}