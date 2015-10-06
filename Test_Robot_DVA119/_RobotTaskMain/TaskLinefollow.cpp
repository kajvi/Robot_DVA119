#include <string.h>
#include "TaskLineFollow.h"
#define C_THIS_TASK "LineFollow 2015-10-06"
// ============================================================================================


void taskLineFollow(struct ioStruct* ptr_io)
{
  strcpy (ptr_io->iosMessageChArr, C_THIS_TASK);
  ptr_io->iosDelayMS = 2000;
} // taskLineFollow

// ============================================================================================

