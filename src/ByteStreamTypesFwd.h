/**
* @file ByteStreamTypesFwd.h
* @brief Forward Declarations for Byte Stream Types Used Within Package
* @authors Frank Reiser
* @date Created on November 18, 2022
*/


#ifndef REISERRT_BYTESTREAMBUF_BYTESTREAMTYPESFWD_H
#define REISERRT_BYTESTREAMBUF_BYTESTREAMTYPESFWD_H

#include <iosfwd>

namespace ReiserRT
{
    namespace Utility
    {
        using InputByteStream = std::basic_istream< unsigned char >;        //!< Alias for Basic Input Byte Stream
        using OutputByteStream = std::basic_ostream< unsigned char >;       //!< Alias for Basic Output Byte Stream
        using InputOutputByteStream = std::basic_iostream< unsigned char >; //!< Alias for Basic Input/Output Byte Stream
    }
}

#endif //REISERRT_BYTESTREAMBUF_BYTESTREAMTYPESFWD_H
