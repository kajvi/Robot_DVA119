#ifndef IO_H
#define IO_H

// Reserves 200-1 characters for a char.array.
#define C_MESSAGE_SIZE 200

//= 0 om ljust, =1 om svart
#define C_DARK_1 1
#define C_LIGHT_0 0

// Sets action of the robot depending on the state of the robot.
enum actionEnum {
  aeActionUnknown,
  aeActionStill,
  aeActionTurnHalfLeft,
  aeActionTurnFullLeft,
  aeActionTurnHalfRight,
  aeActionTurnFullRight,
  aeActionGoForward,
  aeActionSearch
};

// Sets engine directions.
enum directionEnum {
  deUnknown,
  deForward,
  deBackward
};

// Engine struct which contains direction and speed of the engine.
struct engineStruct {
  enum directionEnum direction;
  int speed;
};

// struct I/O struct containing all sensor streams (INPUT) and all engine and Diod streams (OUTPUT)
struct ioStruct {
  // Reflector Streams for LineFollow
  int iosReflFrontLeft_0;
  int iosReflFrontCenter_1;
  int iosReflFrontRight_2;

  // Accelerometer
  int iosAccelerometerX;
  int iosAccelerometerY;

  // Engine Streams
  struct engineStruct iosLeftEngine;
  struct engineStruct iosRightEngine;

  // Led Streams
  int iosLedGreen;
  int iosLedRed;

  // Message: if not empty displayed by main then set to empty
  char iosMessageChArr[C_MESSAGE_SIZE];

  // Delay counter: if not zero in Main call delay(iosDelayMS) then set to zero
  long iosDelayMS;
};


#endif // TASK_LABYRINTH_H
