#include "TaskLineFollow.h"
#define C_THIS_TASK "LineFollow 2015-10-06"
// ============================================================================================

static int stat_test = 123;

int taskLineFollow(struct ioStruct* ptr_io)
{
  stat_test++;
  return stat_test;
} // taskLineFollow

// ============================================================================================

