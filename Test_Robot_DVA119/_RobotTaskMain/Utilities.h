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
//
//
// ============================================================================


#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>
#include "IO.h"

String adjustStr(String i_Str, int i_Width);
frontLCRsensorsEnum decodeFrontLCRsensors(struct ioStruct* ptr_io);

#endif // UTILITIES_H
