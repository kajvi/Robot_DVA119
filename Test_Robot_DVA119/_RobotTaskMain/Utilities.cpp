// Utilities.cpp
//
// Generic utilities
// 
// DVA119 -  Introduktion till robotik, ht'15, MdH
//
// Authors:
// Daniel Stenekap, Göran Forsström
//
// History
// 2015-10-16   Introduced
// 2015-10-19   decodeFrontLCRsensors: fixed handling of 3-bit invert.
// 2015-10-23   Added: setUpLedFromValueLimits
// 2015-10-25   Fixed: adapted to wrong left-right description of sensors + log replaced by bit ops!
// ============================================================================

#include "Utilities.h"

// ============================================================================

String adjustStr(String i_Str, int i_Width)
{
  // i_Width < 0 - add spaces after i_Str  - Left Justify
  // i_Width > 0 - add spaces before i_Str - Right Justify
  // No if i_Str longer that abs(i_Width)  - no change!
  
  String resultStr = i_Str;
  
  if (i_Width > 0)
  {
    // Add spaces before
    while (resultStr.length() < abs(i_Width))
    {
      resultStr = ' ' + resultStr;
    } // while
  } // if
  else
  {
    // Add spaces after
    while (resultStr.length() < abs(i_Width))
    {
      resultStr = resultStr + ' ';
    } // while    
  } // else
  
  return resultStr;
 
} // adjustStr

// ============================================================================

frontLCRsensorsEnum decodeFrontLCRsensors(struct ioStruct* ptr_io)
{
  int tmp;
  
 // tmp = (ptr_io->iosReflFrontLeft_0 << 2) || (ptr_io->iosReflFrontCenter_1 << 1) || (ptr_io->iosReflFrontRight_2);
 // FEL ORDNING PÅ SENSORERNA! Vänster och höger förväxlat jmf labb-beskrivning!
  tmp = (ptr_io->iosReflFrontLeft_0 << 0) | (ptr_io->iosReflFrontCenter_1 << 1) | (ptr_io->iosReflFrontRight_2 << 2);
  return (frontLCRsensorsEnum) ((~tmp & 0x7) + 1); // 0 = dfsUnknown, 1 = dfs_LCR_DarkDarkDark, Dark = 1, Light = 0!
  
} // decodeFrontLCRsensors

// ============================================================================

// Led Colour := OFF, Orange, Red, Green dep. input value

void setUpLedFromValueLimits(int i_Value, int i_LL, int i_LH, int i_HL, int i_HH, int * ptr_LedGreen, int * ptr_LedRed)
{
  if (i_Value < i_LL)
  {
    *ptr_LedGreen = LOW;
    *ptr_LedRed = LOW;
  }
  else if (i_Value < i_LH)
  {
    *ptr_LedGreen = HIGH;
    *ptr_LedRed = HIGH;
  }
  else if (i_Value < i_HL)
  {
    *ptr_LedGreen = HIGH;
    *ptr_LedRed = LOW;
  }
  else if (i_Value < i_HH)
  {
    *ptr_LedGreen = LOW;
    *ptr_LedRed = HIGH;
  }
} // setUpLedFromValueLimits

// ============================================================================


