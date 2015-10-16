// C_TaskBalls.cpp
//
// Handle task searching for coloured balls.
// 
// DVA119 -  Introduktion till robotik, ht'15, MdH
//
// Authors:
// Daniel Stenekap, Göran Forsström
//
// History
// 2015-10-16   Introduced
//
//
// ============================================================================

#include <string.h>
#include "C_TaskBalls.h"

#define C_THIS_TASK "TaskBalls 2015-10-16"

// Sets state of robot: what is State is the robot in.
enum robotStateEnum {
  rsUnknown,
  rsInitial,
  rsStraight,
  rsLeft,
  rsSharpLeft,
  rsRight,
  rsSharpRight,
  rsBackwards,
  rsFinnished
};

static enum robotStateEnum stat_RobotState = rsInitial;

// ============================================================================================

void taskBalls(struct ioStruct* ptr_io)
{
  strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
  ptr_io->iosCurrentTaskIsFinished = 1; 
} // taskBalls

// ============================================================================================


