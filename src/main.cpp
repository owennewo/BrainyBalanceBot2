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

  RemoteSetup();
  PidUpdate(0.0027, 0, 200, 0.5);
  DisplaySetup();
  ImuSetup();
  MotorsSetup();
}

void loop()
{

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
      Serial.print("+");
      bot.angleTarget = PidSpeedToAngle(bot.speed, bot.speedCommand, bot.sampleInterval, bot.crashed);
      bot.speedTarget = PidAngleToSteps(bot.angle, bot.angleTarget, bot.sampleInterval);

      bot.speedTarget = constrain(bot.speedTarget, bot.speed - MAX_ACCEL, bot.speed + MAX_ACCEL);
      bot.speedTarget = constrain(bot.speedTarget, -MAX_SPEED, MAX_SPEED);

      MotorsSetLeftSpeed(bot.speedTarget - bot.steeringCommand);
      MotorsSetRightSpeed(bot.speedTarget + bot.steeringCommand);
    }    
    DisplayLoop(bot);  
  }

}