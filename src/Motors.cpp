#include <Arduino.h>

#define TIMER_PRESCALER_DIV 16

#define MICROSTEPS 32
#define MIN_INTERVAL 100

hw_timer_t * leftTimer = NULL;
hw_timer_t * rightTimer = NULL;
portMUX_TYPE leftTimerMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE rightTimerMux = portMUX_INITIALIZER_UNLOCKED;

const int microsteps = 32; //8; // A4988 is set for 1/8th steps

uint8_t leftDirPin;
uint8_t leftStepPin;
uint8_t rightDirPin;
uint8_t rightStepPin;
uint8_t enablePin;

int leftDir;
int rightDir;

boolean leftHigh;
boolean rightHigh;

volatile int leftStepsCount;
volatile int rightStepsCount;

boolean motorEnabled = true;

void IRAM_ATTR onLeftTimer() {
  portENTER_CRITICAL_ISR(&leftTimerMux);
  leftStepsCount += leftDir;
  portEXIT_CRITICAL_ISR(&leftTimerMux);
  // leftHigh = !leftHigh; 
  // digitalWrite(leftStepPin, leftHigh);
}

void IRAM_ATTR onRightTimer() {
  portENTER_CRITICAL_ISR(&rightTimerMux);
  rightStepsCount  += rightDir;
  portEXIT_CRITICAL_ISR(&rightTimerMux);
  // rightHigh = !rightHigh; 
  // digitalWrite(rightStepPin, rightHigh);
}

void MotorsEnable() {
  if (!motorEnabled) {
    motorEnabled = true;
    Serial.println("MOTOR +");
    digitalWrite(enablePin, LOW);
  }
}

void MotorsDisable() {
  if (motorEnabled) {
    motorEnabled = false;
    Serial.println("MOTOR -");
    digitalWrite(enablePin, HIGH);
  }
}

void MotorsSetup(uint8_t _leftDirPin, uint8_t _leftStepPin, uint8_t _rightDirPin, uint8_t _rightStepPin, uint8_t _enablePin) {
  leftDirPin = _leftDirPin;
  leftStepPin = _leftStepPin;
  rightDirPin = _rightDirPin;
  rightStepPin = _rightStepPin;
  enablePin = _enablePin;

  pinMode(leftStepPin, OUTPUT);
  pinMode(leftDirPin, OUTPUT);
  pinMode(rightDirPin, OUTPUT);
  pinMode(rightStepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  MotorsDisable();

  Serial.println("initialising left timer");
  leftTimer = timerBegin(0, 80, true); // prescaler of 80 means 1Mhz
  timerAttachInterrupt(leftTimer, &onLeftTimer, true);
  timerAlarmWrite(leftTimer, 1000000, true);
  timerAlarmEnable(leftTimer);
  Serial.println("initialising right timer");
  rightTimer = timerBegin(1, 80, true); // prescaler of 80 means 1Mhz
  timerAttachInterrupt(rightTimer, &onRightTimer, true);
  timerAlarmWrite(rightTimer, 1000000, true);
  timerAlarmEnable(rightTimer);

  Serial.println("initialised timer");
}

float MotorsGetSpeed() {


  portENTER_CRITICAL(&leftTimerMux);
  portENTER_CRITICAL(&rightTimerMux);
  float averageSteps = (float) (leftStepsCount + rightStepsCount) / 2;
  leftStepsCount = 0;
  rightStepsCount = 0;
  portEXIT_CRITICAL(&leftTimerMux);
  portEXIT_CRITICAL(&rightTimerMux);
  
  float averageSpeed = averageSteps * 360 / (200 * MICROSTEPS * 2); 
  
  // data.speedAngular = (data.angularVelocity * 2.5);
  // data.speedEstimated = - data.speedMeasured + data.speedAngular;
  // data.speedEstimatedSmoothed = data.speedEstimatedSmoothed * 0.9 + data.speedEstimated * 0.1;
  return averageSpeed;

}

uint8_t MotorsSetSpeed(hw_timer_t * timer, uint8_t dirPin, long degreesPerSecond) {
  if (degreesPerSecond == 0) {
    degreesPerSecond = 1;
  }
  float stepsPerSecond = (float) degreesPerSecond * 200 / 360 * MICROSTEPS;
  // Serial.printf("######### %d %d\n", degreesPerSecond, stepsPerSecond);
  
  long toggleInterval = long(1000000 / (stepsPerSecond * 2));

  if (toggleInterval < MIN_INTERVAL) {
    toggleInterval = MIN_INTERVAL;
    Serial.println("Canna go any faster captain!");
  }
  // Serial.printf("######### %d %f %d\n", degreesPerSecond, stepsPerSecond, toggleInterval);
  // while(true) ;
  timerAlarmWrite(leftTimer, abs(toggleInterval), true);

  if (degreesPerSecond >= 0) {
    digitalWrite(dirPin, LOW);
    return 1;
  } else {
    digitalWrite(dirPin, HIGH);
    return -1;
  }
}

void MotorsSetLeftSpeed(long degreesPerSecond) {
  leftDir = MotorsSetSpeed(leftTimer, leftDirPin, degreesPerSecond);
}

void MotorsSetRightSpeed(long degreesPerSecond) {
  rightDir = MotorsSetSpeed(rightTimer, rightDirPin, degreesPerSecond);
}
