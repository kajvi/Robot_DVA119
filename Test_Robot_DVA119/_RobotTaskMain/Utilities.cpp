// Utilities.cpp
//
// Generic utilities
// 
// DVA119 -  Introduktion till robotik, ht'15, MdH
//
// Authors:
// Daniel
// Göran Forsström
//
// History
// 2015-10-16   Introduced
//
//
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
