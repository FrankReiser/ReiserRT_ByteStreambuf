#include "ByteStreambuf.h"

using namespace ReiserRT::Utility;

ByteStreambuf::ByteStreambuf( char_type * pBuf, std::streamsize len, std::ios_base::openmode _openMode )
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

    ///@todo There is a lot in common between these two blocks and this code could be simplified.
    ///It took me some time to get this working and I have to leave it alone for now.
    ///Danger though. Argument "which" could be both in & out, although I have never witnessed it.
    if ( ( which & std::ios_base::in ) && ( _M_openMode & std::ios_base::in ) )
    {
        // Get current offset
        const std::streampos curOffset = gptr() - eback();

        // If seek off is zero from current position, just return the current offset.
        // This is more of a query used by istream::tellg()
        if ( 0 == off && std::ios_base::cur == way ) retVal = curOffset;

            // Otherwise, seek based on seek direction
        else switch ( way )
        {
            case std::ios_base::cur: retVal = seekpos( curOffset + off, std::ios_base::in ); break;
            case std::ios_base::beg: retVal = seekpos( off, std::ios_base::in ); break;
            case std::ios_base::end: retVal = seekpos( egptr() - eback() + off, std::ios_base::in ); break;
        }
    }

    if ( ( which & std::ios_base::out ) && ( _M_openMode & std::ios_base::out ) )
    {
        // Get current offset
        const std::streampos curOffset = pptr() - pbase();

        // If seek off is zero from current position, just return the current offset.
        if ( 0 == off && std::ios_base::cur == way ) retVal = curOffset;

            // Otherwise, seek based on seek direction
        else switch ( way )
        {
            case std::ios_base::cur: retVal = seekpos( curOffset + off, std::ios_base::out ); break;
            case std::ios_base::beg: retVal = seekpos( off, std::ios_base::out ); break;
            case std::ios_base::end: retVal = seekpos( epptr() - pbase() + off, std::ios_base::out ); break;
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