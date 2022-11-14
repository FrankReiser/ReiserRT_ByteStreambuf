/**
* @file ByteStreambuf.h
* @brief The Specification for a ByteStream Buffer Utility.
* @authors Frank Reiser
* @date Created on November 13, 2022
*/


#include <iostream>

namespace ReiserRT
{
    namespace Utility
    {
        ///@todo Export class
        class ByteStreambuf : public std::basic_streambuf< unsigned char >
        {
        public:
            /**
            * @brief Constructor for ByteStreamBuf
            *
            * This constructor initializes the stream buffer, storing the "open mode" and setting up
            * the get and/or put, stream buffer pointers.
            *
            * @param pBuf A pointer to octet block (unsigned char - byte) to be utilized for buffering.
            * @param len The length of the octet block.
            * @param _openMode The open mode, defaults to std::ios_base::in | std::ios_base::out.
            */
            explicit ByteStreambuf( char_type * pBuf, std::streamsize len,
                        std::ios_base::openmode _openMode = std::ios_base::in | std::ios_base::out );

        protected:
            /**
            * @brief Set the Buffer for ByteStreamBuf
            *
            * This operation sets up the buffer pointers for get and/or put operations dependent on the
            * open mode.
            *
            * @param pBuf A pointer to the buffer memory to be utilized for buffering.
            * @param len The length of the buffer to be utilized for buffering.
            * @return Returns a pointer to the ByteStreamBuf object operated on.
            */
            ByteStreambuf * setbuf( char_type * pBuf, std::streamsize len );


            ///@todo Document as an override
            std::streampos seekoff( std::streamoff off, std::ios_base::seekdir way,
                        std::ios_base::openmode which );


            ///@todo Document as an override
            std::streampos seekpos( std::streampos pos, std::ios_base::openmode which );

        protected:
            /**
            * @brief The open mode.
            *
            * We record the "open mode" at time of construction for later use.
            */
            std::ios_base::openmode _M_openMode;
        };


    }
}