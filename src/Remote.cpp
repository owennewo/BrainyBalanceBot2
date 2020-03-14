#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define INTERRUPT_ATTR IRAM_ATTR

BluetoothSerial SerialBT;

void RemoteSetup() {
    Serial.println("setting up BT");
    SerialBT.begin("BrainyBalanceBot");
    Serial.println("Bluetooth started!");
}

void RemoteLoop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}