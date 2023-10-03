#include "ByteStreambuf.h"

using namespace ReiserRT::Utility;

ByteStreambuf::ByteStreambuf( char_type * pBuf, std::streamsize len, std::ios_base::openmode _openMode )
  : std::basic_streambuf< unsigned char >()
  , _M_openMode( _openMode )
{
    if ( _M_openMode & std::ios_base::in )
        setg(pBuf, pBuf, pBuf + len );
    if ( _M_openMode & std::ios_base::out )
        setp(pBuf, pBuf + len );
}

ByteStreambuf * ByteStreambuf::setbuf( char_type * pBuf, std::streamsize len )
{
    if ( _M_openMode & std::ios_base::in )
        setg(pBuf, pBuf, pBuf + len );
    if ( _M_openMode & std::ios_base::out )
        setp(pBuf, pBuf + len );

    return this;
}

std::streampos ByteStreambuf::seekoff( std::streamoff off, std::ios_base::seekdir way,
                        std::ios_base::openmode which )
{
    std::streampos retVal = -1;

    // If performing input
    if ( ( which & std::ios_base::in ) && ( _M_openMode & std::ios_base::in ) )
    {
        // Get current offset
        const std::streampos curOffset = gptr() - eback();

        // If seek off is zero from current position, just return the current offset.
        // This is more of a query used by istream::tellg()
        if ( 0 == off && std::ios_base::cur == way ) retVal = curOffset;

        // Otherwise, seek based on seek direction
        else {
            if ( std::ios_base::cur == way ) retVal = seekpos( curOffset + off, std::ios_base::in );
            else if ( std::ios_base::beg == way ) retVal = seekpos( off, std::ios_base::in );
            else if ( std::ios_base::end == way ) retVal = seekpos( egptr() - eback() + off, std::ios_base::in );
        }
    }

    // If performing output
    if ( ( which & std::ios_base::out ) && ( _M_openMode & std::ios_base::out ) )
    {
        // Get current offset
        const std::streampos curOffset = pptr() - pbase();

        // If seek off is zero from current position, just return the current offset.
        if ( 0 == off && std::ios_base::cur == way ) retVal = curOffset;

        // Otherwise, seek based on seek direction
        else {
            if ( std::ios_base::cur == way ) retVal = seekpos( curOffset + off, std::ios_base::out );
            else if ( std::ios_base::beg == way ) retVal = seekpos( off, std::ios_base::out );
            else if ( std::ios_base::end == way ) retVal = seekpos( epptr() - pbase() + off, std::ios_base::out );
        }
    }

    return retVal;
}

std::streampos ByteStreambuf::seekpos( std::streampos pos, std::ios_base::openmode which )
{
    std::streampos retVal = -1;

    if ( ( which & std::ios_base::in ) && ( _M_openMode & std::ios_base::in ) )
    {
        const std::streampos curOffset = gptr() - eback();
        if ( curOffset == pos )
        {
            retVal = curOffset;
        }
        else if ( 0 <= pos && egptr() > ( eback() + pos ) )
        {
            gbump( int( pos - curOffset ) );
            retVal = pos;
        }
    }

    if ( ( which & std::ios_base::out ) && ( _M_openMode & std::ios_base::out ) )
    {
        const std::streampos curOffset = pptr() - pbase();
        if ( curOffset == pos )
        {
            retVal = curOffset;
        }
        else if ( 0 <= pos && epptr() > ( pbase() + pos ) )
        {
            pbump( int( pos - curOffset ) );
            retVal = pos;
        }
    }

    return retVal;
}