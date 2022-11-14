/**
* @file TestData.h
* @brief Specification File for Test Data to Verify Serialization and ByteStreambuf
* @authors Frank Reiser
* @date Created on November 14, 2022
*/

#ifndef REISERRT_BYTESTREAMBUF_TESTDATA_H
#define REISERRT_BYTESTREAMBUF_TESTDATA_H

// Raw Network Ordered Bytes for testing
// This data has to be non-const as we allow writing to blocks of data. Please don't attempt to overwrite the test data.
extern unsigned char testData[8];

#if 0
// Contents of the Raw Network Ordered bytes when Properly Deserialized.
constexpr unsigned short uShortValOut = 33922;
constexpr signed short shortValOut = -31613;
constexpr unsigned int uIntValOut = 2223177856;
constexpr signed int intValOut = -2071789439;
constexpr float floatValOut = 48.375244140625;
constexpr double doubleValOut = 150332260384.0626220703125;
#endif

#endif //REISERRT_BYTESTREAMBUF_TESTDATA_H
