/**
* @file ByteStreambuf.h
* @brief The Specification for a ByteStream Buffer Utility.
* @authors Frank Reiser
* @date Created on November 13, 2022
*/


#include <iostream>
#include <endian.h>

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


        /**
        * @brief Deserialize network ordered bytes from byte stream into a type
        *
        * This helper template operation converts network ordered bytes from the byte stream into type T.
        *
        * @tparam T Type T is the type to convert to.
        * @param byteStream A reference to the byte stream containing the network ordered bytes.
        * @return The number of bytes deserialized which may be less than the size of T if the stream does not remain
        * in the "good" state for the entire operation.
        */
        template < typename T >
        size_t _deserializeFromByteStream( std::basic_istream< unsigned char > & byteStream, unsigned char * pType )
        {
            static_assert( std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_enum<T>::value,
                           "Type T must be an integer, floating point or enumerator type" );
            size_t i = 0;

#if ( __BYTE_ORDER == __BIG_ENDIAN )
            for ( ; byteStream.good() && i != sizeof ( T ); ++i )
                byteStream.get( *pType++ );
#elif ( __BYTE_ORDER == __LITTLE_ENDIAN )
            pType += sizeof( T ) - 1;
            for ( ; byteStream.good() && i != sizeof ( T ); ++i )
                byteStream.get( *pType-- );
#else
#error "Preprocessor symbol __BYTE_ORDER must be defined as __BIG_ENDIAN or __LITTLE_ENDIAN!!!"
#endif
            return i;
        }

        /**
        * @brief Deserialize network ordered bytes from an array into a type
        *
        * This helper template operation converts network ordered bytes from an array into type T.
        *
        * @tparam T Type T is the type to convert to. It must be a numeric or enumerator type.
        * @param bytes A pointer to the array containing the network ordered bytes.
        * @return Returns the number of bytes deserialized which will be the sizeof( T ).
        */
        template < typename T >
        size_t _deserializeFromByteArray( const unsigned char * pByteBlock, unsigned char * pType )
        {
            static_assert( std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_enum<T>::value,
                           "Type T must be an integer, floating point or enumerator type" );

            size_t i = 0;
#if ( __BYTE_ORDER == __BIG_ENDIAN )
            for ( ; i != sizeof ( T ); ++i )
                *pType++ = *pByteBlock++;
#elif ( __BYTE_ORDER == __LITTLE_ENDIAN )
            pType += sizeof( T ) - 1;
            for ( ; i != sizeof ( T ); ++i )
                *pType-- = *pByteBlock++;
#else
#error "Preprocessor symbol __BYTE_ORDER must be defined as __BIG_ENDIAN or __LITTLE_ENDIAN!!!"
#endif
            return i;
        }

        /**
        * @brief Serialize a type onto a network ordered byte stream.
        *
        * This helper template operation converts a type of type T onto a network ordered byte stream.
        *
        * @tparam T Type T is the type to convert from. It must be a numeric or enumerator type.
        * @param t The value to serialize.
        * @param byteStream A reference to the byte stream where the network ordered bytes will be written to.
        * @return The number of bytes serialized which may be less than the size of T if the stream does not remain
        * in the "good" state for the entire operation.
        */
        template < typename T >
        size_t _serializeToByteStream( std::basic_ostream< unsigned char > & byteStream, const unsigned char * pType )
        {
            static_assert( std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_enum<T>::value,
                           "Type T must be an integer, floating point or enumerator type" );

            size_t i = 0;
#if ( __BYTE_ORDER == __BIG_ENDIAN )
            for ( ; byteStream.good() && i != sizeof ( T ); ++i )
                byteStream.put( *pType++ );
#elif ( __BYTE_ORDER == __LITTLE_ENDIAN )
            pType += sizeof( T ) - 1;
            for ( ; byteStream.good() && i != sizeof ( T ); ++i )
                byteStream.put( *pType-- );
#else
#error "Preprocessor symbol __BYTE_ORDER must be defined as __BIG_ENDIAN or __LITTLE_ENDIAN!!!"
#endif
            return i;
        }

        /**
        * @brief Serialize a type onto a network ordered byte array.
        *
        * This helper template operation converts a type of type T onto a network ordered byte array.
        *
        * @tparam T Type T is the type to convert from. It must be a numeric or enumerator type.
        * @param t The value to serialize.
        * @param byte A reference to the byte array where the network ordered bytes will be written to.
        * @return Returns the number of bytes serialized which will be the sizeof( T ).
        */
        template < typename T >
        size_t _serializeToByteArray( unsigned char * pByteBlock, const unsigned char * pType )
        {
            static_assert( std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_enum<T>::value,
                           "Type T must be an integer, floating point or enumerator type" );

            size_t i = 0;
#if ( __BYTE_ORDER == __BIG_ENDIAN )
            for ( ; i != sizeof ( T ); ++i )
                *pByteBlock++ = *pType++;
#elif ( __BYTE_ORDER == __LITTLE_ENDIAN )
            pType += sizeof( T ) - 1;
            for ( ; i != sizeof ( T ); ++i )
                *pByteBlock++ = *pType--;
#else
#error "Preprocessor symbol __BYTE_ORDER must be defined as __BIG_ENDIAN or __LITTLE_ENDIAN!!!"
#endif
            return i;
        }

        /**
        * @brief Convert network ordered bytes from byte stream into a type
        *
        * This template operation converts network ordered bytes from the byte stream into type T.
        *
        * @tparam T Type T is the type to convert to. It must be a numeric or enumerator type.
        * @param byteStream A reference to the byte stream containing the network ordered bytes.
        * @return Returns value type T.
        * @warning The type may not be fully deserialized if the stream hits EOF before all bytes are deserialized.
        * The client should guard against this possibility by modifying the stream exception mask to throw an std::ios_base::failure
        * exception should std::ios_base::badbit or std::ios_base::eofbit become asserted.
        */
        template < typename T >
        T netToType( std::basic_istream< unsigned char > & byteStream )
        {
            union { unsigned char buf[ sizeof ( T ) ]; T t; } u;
            _deserializeFromByteStream< T >( byteStream, u.buf );
            return u.t;
        }

        /**
        * @brief Convert network ordered bytes from byte stream into a type
        *
        * This template operation converts network ordered bytes from the byte stream into type T.
        *
        * @tparam T Type T is the type to convert to. It must be a numeric or enumerator type.
        * @param byteStream A reference to the byte stream containing the network ordered bytes.
        * @param t The deserialized value.
        * @return The number of bytes deserialized.
        * @warning The type may not be fully deserialized if the stream hits EOF before all bytes are deserialized.
        * The client should guard against this possibility by modifying the stream exception mask to throw an std::ios_base::failure
        * exception should std::ios_base::badbit or std::ios_base::eofbit become asserted.
        */
        template < typename T >
        size_t netToType( std::basic_istream< unsigned char > & byteStream, T & t )
        {
            return _deserializeFromByteStream< T >( byteStream, reinterpret_cast< unsigned char * >( &t ) );
        }

        /**
        * @brief Convert network ordered bytes from an array of bytes into a type
        *
        * This template operation converts network ordered bytes from an array of byte into type T.
        *
        * @tparam T Type T is the type to convert to. It must be a numeric or enumerator type.
        * @param bytes A pointer to the array containing the network ordered bytes.
        * @return Returns value type T.
        * @warning It is the clients responsibility to ensure that the array is not read beyond its bounds.
        */
        template < typename T >
        T netToType( unsigned char * bytes )
        {
            union { unsigned char buf[ sizeof ( T ) ]; T t; } u;
            _deserializeFromByteArray< T >( bytes, u.buf );
            return u.t;
        }

        /**
        * @brief Convert network ordered bytes from an array of bytes into a type
        *
        * This template operation converts network ordered bytes from an array of byte into type T.
        *
        * @tparam T Type T is the type to convert to. It must be a numeric or enumerator type.
        * @param bytes A pointer to the array containing the network ordered bytes.
        * @param t The deserialized value.
        * @return The number of bytes deserialized.
        * @warning It is the clients responsibility to ensure that the array is not read beyond its bounds.
        */
        template < typename T >
        size_t netToType( unsigned char * bytes, T & t )
        {
            return _deserializeFromByteArray< T >( bytes, reinterpret_cast< unsigned char * >( &t ) );
        }

        /**
        * @brief Convert a type onto a network ordered byte stream.
        *
        * This template operation converts a type of type T onto a network ordered byte stream.
        *
        * @tparam T Type T is the type to convert from. It must be a numeric or enumerator type.
        * @param t The value to serialize.
        * @param byteStream A reference to the byte stream where the network ordered bytes will be written to.
        * @return The number of bytes serialized.
        */
        template < typename T >
        size_t typeToNet( const T & t, std::basic_ostream< unsigned char > & byteStream )
        {
            return _serializeToByteStream< T >( byteStream, reinterpret_cast< const unsigned char * >( &t ) );
        }

        /**
        * @brief Convert a type onto a network ordered byte array.
        *
        * This template operation converts a type of type T onto a network ordered byte array.
        *
        * @tparam T Type T is the type to convert from. It must be a numeric or enumerator type.
        * @param t The value to serialize.
        * @param byte A reference to the byte array where the network ordered bytes will be written to.
        * @return The number of bytes serialized.
        * @warning The type may not be fully serialized if the stream hits EOF before all bytes are serialized.
        * The client should guard against this possibility by modifying the stream exception mask to throw an std::ios_base::failure
        * exception should std::ios_base::badbit or std::ios_base::eofbit become asserted.
        */
        template < typename T >
        size_t typeToNet( const T & t, unsigned char * bytes )
        {
            return _serializeToByteArray< T >( bytes, reinterpret_cast< const unsigned char * >( &t ) );
        }

    }
}