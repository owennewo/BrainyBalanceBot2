// =======
// TUNINGS
// =======
#define MAX_ACCEL 15
#define MAX_SPEED 720

#define ITERM_MAX_ERROR 300   // Iterm windup constants for PI control 
#define ITERM_MAX 50000

#define CRASH_ANGLE 20

#define STEPPER_MICROSTEPS 32  //1-32 for DRV8825, OR 1-16 for A4988
#define MIN_STEPPER_INTERVAL 20  // if you go too fast you may have too many interrupts

// =====
// TIMER
// =====
#define TIMER_BASE_SPEED 80000000 // 80 MHz
#define TIMER_PRESCALER_DIVIDER 80    
#define TIMER_SPEED TIMER_BASE_SPEED / TIMER_PRESCALER_DIVIDER // 1 MHz

// ===============
// PIN DEFINITIONS
// ===============
#define PIN_BUTTON_1 35
#define PIN_BUTTON_2 0

// STEPPER PINS
#define PIN_MOTOR_LEFT_STEP 26
#define PIN_MOTOR_LEFT_DIR 25
#define PIN_MOTOR_RIGHT_STEP 32
#define PIN_MOTOR_RIGHT_DIR 33
#define PIN_MOTOR_ENABLE 15
// Next pins arent't really connected
#define PIN_IMU_GND 17
#define PIN_IMU_VCC 2
#define PIN_WIRE_SDA 21
#define PIN_WIRE_SCL 22

// I2C PINS
// Note: SDA/SCL are using defaults
// IMU PINS
#define PIN_IMU_INTERRUPT 36

//TFT PINS
#define PIN_TFT_MOSI            19
#define PIN_TFT_SCLK            18
#define PIN_TFT_CS              5
#define PIN_TFT_DC              16
#define PIN_TFT_RST             23
#define PIN_TFT_BL          4  // Display backlight control pin

// BATTERY PINS
// #define ADC_EN          14
#define PIN_BATTERY_ADC         34

// =========
// CONSTANTS
// =========
#define M_PI		3.14159265358979323846





