#ifndef IO_H
#define IO_H

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
  int iosReflFrontLeft;
  int iosReflFrontCenter;
  int iosReflFrontRight;

  // Engine Streams
  struct engineStruct iosLeftEngine;
  struct engineStruct iosRightEngine;

  // Led Streams
  int iosLedGreen;
  int iosLedRed;
};


#endif // TASK_LABYRINTH_H
