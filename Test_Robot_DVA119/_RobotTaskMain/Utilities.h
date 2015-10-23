// Utilities.h
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
// 2015-10-23   Added: setUpLedFromValueLimits
//
// ============================================================================


#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>
#include "IO.h"

String adjustStr(String i_Str, int i_Width);
frontLCRsensorsEnum decodeFrontLCRsensors(struct ioStruct* ptr_io);

void setUpLedFromValueLimits(int i_Value, int i_LL, int i_LH, int i_HL, int i_HH, int * ptr_LedGreen, int * ptr_LedRed);

#endif // UTILITIES_H
