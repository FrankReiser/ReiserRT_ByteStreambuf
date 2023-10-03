/**
* @file TestData.cpp
* @brief Implementation File for Test Data To Verify Serialization and ByteStreambuf
* @authors Frank Reiser
* @date Created on November 14, 2022
*/

#include "TestData.h"

unsigned char testData[16] = { 0x42, 0x41, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const unsigned short uShortTestVal1 = 0x4241;
const unsigned short uShortTestVal2 = 0x8040;
const signed short sShortTestVal = (signed short)0x4241;
const unsigned int uIntTestVal = 0x42418040;
const unsigned int sIntTestVal = (signed int)uIntTestVal;
const unsigned long uLongTestVal = 0x4241804020100804;
const unsigned long sLongTestVal = (signed long)uLongTestVal;
const float floatTestVal = 48.375244140625;
const double doubleTestVal = 150332260384.0626220703125;

