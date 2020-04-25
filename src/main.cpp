#include <Arduino.h>

#include <User_Defines.h> 
#include <Display.h>

#include <Button2.h>
#include <Imu.h>
#include <PID.h>
#include <Motors.h>
#include <Remote.h>
#include <Data.h>


Button2 btn1(PIN_BUTTON_1);
Button2 btn2(PIN_BUTTON_2);

BotState bot;




void setup()
{
  Serial.begin(115200);
  delay(1000);
  RemoteSetup();
  PidUpdate(0.0027, 0, 200, 0.5);
  DisplaySetup();
  ImuSetup();
  MotorsSetup();
  // pinMode(PIN_MOTOR_ENABLE, OUTPUT);
  // pinMode(PIN_MOTOR_LEFT_DIR, OUTPUT);
  // pinMode(PIN_MOTOR_LEFT_STEP, OUTPUT);
  // pinMode(PIN_MOTOR_RIGHT_DIR, OUTPUT);
  // pinMode(PIN_MOTOR_RIGHT_STEP, OUTPUT);
  // // pinMode(PIN_IMU_VCC, OUTPUT);

  // // digitalWrite(PIN_IMU_VCC, HIGH);

  // int delayMicros = 50;
  // int steps = 200 * 32;
  // digitalWrite(PIN_MOTOR_ENABLE, LOW);
  // digitalWrite(PIN_MOTOR_LEFT_DIR, LOW);
  // digitalWrite(PIN_MOTOR_RIGHT_DIR, LOW);

  // Serial.println("Disabled");
  // digitalWrite(PIN_MOTOR_ENABLE, HIGH);
  // delay(5000);
  // digitalWrite(PIN_MOTOR_ENABLE, LOW);
  // Serial.println("Forwards Start");
  // for (int i = 0; i < steps; i++) {
  //   digitalWrite(PIN_MOTOR_LEFT_STEP, LOW);
  //   digitalWrite(PIN_MOTOR_RIGHT_STEP, LOW);
  //   delayMicroseconds(delayMicros);
  //   digitalWrite(PIN_MOTOR_LEFT_STEP, HIGH);
  //   digitalWrite(PIN_MOTOR_RIGHT_STEP, HIGH);
  //   delayMicroseconds(delayMicros);
  // }
  // Serial.println("Forwards end");
  // digitalWrite(PIN_MOTOR_ENABLE, HIGH);
  // delay(5000);
  // Serial.println("Backward Start");
  // digitalWrite(PIN_MOTOR_ENABLE, LOW);
  
  // digitalWrite(PIN_MOTOR_LEFT_DIR, HIGH);
  // digitalWrite(PIN_MOTOR_RIGHT_DIR, HIGH);
  // for (int i = 0; i < steps; i++) {
  //   digitalWrite(PIN_MOTOR_LEFT_STEP, LOW);
  //   digitalWrite(PIN_MOTOR_RIGHT_STEP, LOW);
  //   delayMicroseconds(delayMicros);
  //   digitalWrite(PIN_MOTOR_LEFT_STEP, HIGH);
  //   digitalWrite(PIN_MOTOR_RIGHT_STEP, HIGH);
  //   delayMicroseconds(delayMicros);
  // }
  // Serial.println("Backward End");
  
  // digitalWrite(PIN_MOTOR_ENABLE, HIGH);
  
  // Serial.println("Done - disabled");
  

}

void loop() {

// }

// void loop2() {

  if (RemoteHasCommands()) {
    bot.steeringCommand = RemoteGetSteeringCommand();
    bot.speedCommand = RemoteGetSpeedCommand();
  }

  if (ImuHasData())
  {
    bot.setAngle(ImuGetAngle());
    bot.speed = MotorsGetSpeed(bot.sampleInterval);
    
    if (bot.crashed || bot.sampleInterval == 0) {
      MotorsDisable();
      Serial.print("-");
    } else {
      MotorsEnable();
      Serial.print(".");
      bot.angleTarget = PidSpeedToAngle(bot.speed, bot.speedCommand, bot.sampleInterval, bot.crashed);
      bot.speedTarget = PidAngleToSpeed(bot.angle, bot.angleTarget, bot.sampleInterval);

      
      MotorsSetLeftSpeed(bot.speedTarget - bot.steeringCommand);
      MotorsSetRightSpeed(bot.speedTarget + bot.steeringCommand);
    }    
    DisplayLoop(bot);  
  }

}