#include <Arduino.h>
#include <User_Defines.h>

enum Move {
  FORWARDS = 1,
  STOP = 0,
  BACKWARDS = -1
};

hw_timer_t * leftTimer = NULL;
hw_timer_t * rightTimer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

Move leftMove; 
Move rightMove; 

int leftHigh = 0;
int rightHigh = 0;

volatile int leftStepsCount;
volatile int rightStepsCount;

boolean motorEnabled = false;

void IRAM_ATTR onLeftTimer() {
  if (leftMove != STOP) {
    portENTER_CRITICAL_ISR(&timerMux);
    leftHigh = 1 -(leftHigh);
    digitalWrite(PIN_MOTOR_LEFT_STEP, leftHigh);
    // Serial.print(leftHigh);
    leftStepsCount += leftMove;
    portEXIT_CRITICAL_ISR(&timerMux);
  }
}

void IRAM_ATTR onRightTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  rightHigh = 1 -(rightHigh);
  digitalWrite(PIN_MOTOR_RIGHT_STEP, rightHigh);
  rightStepsCount  += rightMove;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void MotorsEnable() {
  if (!motorEnabled) {
    Serial.println("\nOK!");
    motorEnabled = true;
    digitalWrite(PIN_MOTOR_ENABLE, LOW);
    timerAlarmEnable(leftTimer);
    timerAlarmEnable(rightTimer);
  }
}

void MotorsDisable() {
  if (motorEnabled) {
    motorEnabled = false;
    Serial.println("\nCRASH!");
    digitalWrite(PIN_MOTOR_ENABLE, HIGH);
    timerAlarmDisable(leftTimer);
    timerAlarmDisable(rightTimer);
  }
}

long speedToInterval(long degreesPerSecond) {
  float stepsPerSecond = (float) (2 * degreesPerSecond * 200 * STEPPER_MICROSTEPS) / 360;
  // Serial.printf("######### %d %f\n", degreesPerSecond, stepsPerSecond);
  
  if (stepsPerSecond == 0) {
    stepsPerSecond = 1;
  }

  long toggleInterval = abs(long(TIMER_SPEED / stepsPerSecond));

  if (toggleInterval < MIN_STEPPER_INTERVAL) {
    Serial.printf("Warn: %lu is below minimum interval of %lu\n", toggleInterval, (long) MIN_STEPPER_INTERVAL);
    toggleInterval = MIN_STEPPER_INTERVAL;
  }
  return toggleInterval;
}

Move MotorsSetDirection(uint8_t dirPin, long degreesPerSecond) {

  if (degreesPerSecond == 0) {
    return STOP;
  } else if (degreesPerSecond > 0) {
    digitalWrite(dirPin, LOW);
    return FORWARDS;
  } else {
    digitalWrite(dirPin, HIGH);
    return BACKWARDS;
  }
}

void MotorsSetLeftSpeed(long degreesPerSecond) {
  long toggleInterval = speedToInterval(degreesPerSecond);
  timerAlarmWrite(leftTimer, toggleInterval, true);
  leftMove = MotorsSetDirection(PIN_MOTOR_LEFT_DIR, degreesPerSecond);
}

void MotorsSetRightSpeed(long degreesPerSecond) {
  long toggleInterval = speedToInterval(degreesPerSecond);
  timerAlarmWrite(rightTimer, toggleInterval, true);
  Serial.printf(" ### %5d %5d ", degreesPerSecond, toggleInterval);
  rightMove = MotorsSetDirection(PIN_MOTOR_RIGHT_DIR, degreesPerSecond);
}

void MotorsSetup() {
  
  pinMode(PIN_MOTOR_LEFT_STEP, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_DIR, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_STEP, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_DIR, OUTPUT);
  pinMode(PIN_MOTOR_ENABLE, OUTPUT);

  Serial.println("initialising left timer");
  leftTimer = timerBegin(1, TIMER_PRESCALER_DIVIDER, true); // prescaler of 80 means 1Mhz
  timerAttachInterrupt(leftTimer, &onLeftTimer, true);
  MotorsSetLeftSpeed(0);
  
  Serial.println("initialising right timer");
  rightTimer = timerBegin(0, TIMER_PRESCALER_DIVIDER, true); // prescaler of 80 means 1Mhz
  timerAttachInterrupt(rightTimer, &onRightTimer, true);
  MotorsSetRightSpeed(0);
  
  Serial.println("initialised timer");
}

float MotorsGetSpeed(float sampleInterval) {

  portENTER_CRITICAL(&timerMux);
  float averageSteps = (float) (leftStepsCount + rightStepsCount) / 2;
  float averageSpeed = averageSteps * 360 / (200 * STEPPER_MICROSTEPS * 2 * sampleInterval);
  // Serial.printf("============================%6d (%2d) + %6d (%2d) / 2 = %6.1f :%6.1f\n", leftStepsCount, leftDir, rightStepsCount, rightDir, averageSteps, averageSpeed);
  leftStepsCount = 0;
  rightStepsCount = 0;
  portEXIT_CRITICAL(&timerMux);
  
  return averageSpeed;
}

