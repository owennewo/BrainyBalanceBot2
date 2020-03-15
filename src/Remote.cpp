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

char commands[30] = "\n";
char * command;

int speedCommand;
int steeringCommand;
// char * commandValue;
byte commandIndex = 0;

// void processCommand(char commandCode, char * commandValue) {

//     switch(commandCode) {
//        case 's':
//         data.speedInput = atof(commandValue);
//         // lastSteerTime = millis();
//         break;
//       case 'd':
//         data.directionInput = atoi(commandValue);
//         // lastSteerTime = millis();
//         break;
//       case 'P':
//         data.angleKp = atof(commandValue);
//         // Serial.println(data.angleKp);
//         break;
//       // case 'p':
//       //   data.angleKp -= 1;
//       //   break;
//       case 'D':
//         data.angleKd = atof(commandValue);
//         break;
//       // case 'd':
//       //   data.angleKd -= 0.025;
//       //   break;
//       case 'p':
//         data.speedKp =atof(commandValue);
//         break;
//       // case 's':
//       //   data.speedKp -= 0.001;
//       //   break;
//       case 'l':
//         data.log = boolean(atoi(commandValue));
//         break;
//       // case 's':
//       //   data.speedKp -= 0.001;
//       //   break;
//       case 'i':
//         data.speedKi = atof(commandValue);
//         break;
//       case 'A':
//         break;
//   }
// }

long RemoteGetSteeringCommand() {
  return ((steeringCommand - 50) * 8);
}

long RemoteGetSpeedCommand() {
  return -((speedCommand - 50) * 8);
}

void processCommand(char * command) {
  switch (command[0]) {
    case 'M': {
      char * cSteering = strtok(commands, "MXY#");
      char * cSpeed = strtok(NULL, "MXY#");
      speedCommand = atoi(cSpeed);
      steeringCommand = atoi(cSteering);
      Serial.printf("Steering=%d, Speed=%d", steeringCommand, speedCommand);
      break;
    }
    default: {
      Serial.printf("unknown command %c", command[0]);
    }
      
  }
}

bool RemoteHasCommands() {

  while (SerialBT.available()) {
    // Serial.print(".");
    char received = SerialBT.read();
    // Serial.print(received);
    commands[commandIndex] = received; 
    // Serial.print("+");
    commandIndex++;
    // Process message when new line character is recieved
    if (received == '#')
    {
      // Serial.print("-");
      // SerialBT.println("Arduino Received: ");
      // SerialBT.println(command);
      command = strtok(commands, ",#"); // must be single char please!
      while (command != NULL)
      {
        // Serial.print("-");
        Serial.printf ("\ncommand:%s\n",command);
        processCommand(command);
        command = strtok (NULL, ",\n");
      }
      
      // if (commandName != '\0') {
      //   commandValue = strtok(NULL, "\n");
      //   if (commandName != '\0') {
      //     Serial.print((char) commandName);
      //     Serial.print(":");
      //     Serial.print((int) commandName);
      //     Serial.print(":");
      //     Serial.println(commandValue);
      //     Serial.println(command);
      //     processCommand(commandName, commandValue);
      //   } else {
      //     Serial.print("Unknown commandValue: ");
      //     Serial.println(command);
      //   }
      // } else {
      //   Serial.print("Unknown commandName: ");
      //   Serial.println(command);
      // }
      commandIndex = 0;
      return true;   
    }
  }
  return false;
}