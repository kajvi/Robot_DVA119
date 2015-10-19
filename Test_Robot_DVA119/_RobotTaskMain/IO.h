// IO.h
//
// Common data types between main and the different task modules.
// 
// DVA119 -  Introduktion till robotik, ht'15, MdH
//
// Authors:
// Daniel Stenekap, Göran Forsström
//
// History
// 2015-10-16   Introduced
// 2015-10-19   Added dcsStandStill
//
// ============================================================================


#ifndef IO_H
#define IO_H

// Reserves 200 characters for a char.array.
#define C_MESSAGE_SIZE 200

//= 0 om ljust, =1 om svart
#define C_DARK_1 1
#define C_LIGHT_0 0

// Decoded front senors: Left Center, Right
enum frontLCRsensorsEnum {
  dfsUnknown,
  
  dfs_LCR_DarkDarkDark,
  dfs_LCR_DarkDarkLight,
  dfs_LCR_DarkLightDark,
  dfs_LCR_DarkLightLight,

  dfs_LCR_LightDarkDark,
  dfs_LCR_LightDarkLight,
  dfs_LCR_LightLightDark,
  dfs_LCR_LightLightLight
};

// Possible commands to turn robot
enum directionCommandEnum {
    dceUnknown,
    dceTurnLeft,
    dceGoStraight,
    dceTurnRight,
    dcsStandStill
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


// struct I/O struct containing all sensor streams (INPUT) and all activators (engines and Led etc) streams (OUTPUT)

struct ioStruct {
  
  // Sensors
  // =======
  
  // Reflector Streams for LineFollow
  int iosReflFrontLeft_0;
  int iosReflFrontCenter_1;
  int iosReflFrontRight_2;

  // Accelerometer
  int iosAccelerometerX;
  int iosAccelerometerY;


  // Actuators
  // =========
  
  // Engine Streams
  struct engineStruct iosLeftEngine;
  struct engineStruct iosRightEngine;

  // Led Streams
  int iosLedGreen;
  int iosLedRed;


  // Status
  // ======

  unsigned long iosElapsedTime;
  
  int iosCurrentTaskIsFinished; // <> 0 if task finished, else = 0 if task in progress
  
  // Message: if not empty displayed by main then set to empty
  char iosMessageChArr[C_MESSAGE_SIZE];
  int iosMessageInteger = -1;

  // Delay counter: if not zero in Main call delay(iosDelayMS) then set to zero
  long iosDelayMS;
};


#endif // TASK_LABYRINTH_H
