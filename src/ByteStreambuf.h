/**
* @file ByteStreambuf.h
* @brief The Specification for a ByteStream Buffer Utility.
* @authors Frank Reiser
* @date Created on November 13, 2022
*/

#include "ReiserRT_ByteStreambufExport.h"

#include <iostream>

namespace ReiserRT
{
    namespace Utility
    {
        /**
        * @brief Byte Stream Buffer Implementation
        *
        * This class affords the ability to use iostream instances with raw bytes of user provided memory.
        * Note that this is an extremely simple implementation. The initial intent was to provide for simple unformatted
        * reading of a user provided buffer, such as a received UDP datagram and it does this well.
        * Writing has also been proven to work. Some abstract functionality of the standard basic_streambuf is per the default
        * implementation.
        *
        * Of particular note, this class does not take ownership of the user provided memory.
        * This affords a lot of flexibility, but places responsibility on the user to deal with any
        * lifetime issues of such memory. Additionally, it makes no attempts to grow, copy,
        * move, delete, or deal with underflow/overflow issues. Underflow and overflow
        * are treated merely as EOF conditions.
        *
        * @warning The type may not be fully deserialized if the stream hits EOF before all bytes are deserialized.
        * The client should guard against this possibility by modifying the stream exception mask to throw an std::ios_base::failure
        * exception should std::ios_base::badbit or std::ios_base::eofbit become asserted.
        * This would be accomplished by modifying the stream using this buffer, not the buffer itself.
        * See test harness code for use cases.
        *
        * Utilizing it is extremely simple.  Instantiate it with the address and size of
        * the raw memory (local stack object works fine), then construct a std::basic_istream< unsigned char >
        * passing the ByteStreamBuf objects address as such:
        *
        * @code ByteStreamBuf byteStreamBuf( pChar, length );
        * @code std::basic_istream< unsigned char > byteStream( &byteStreamBuf );
        * @endcode
        *
        * Now you can do reads over the space with robust EOF detection although the
        * initial intent was unformatted input.
        */
        class ReiserRT_ByteStreambuf_EXPORT ByteStreambuf : public std::basic_streambuf< unsigned char >
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