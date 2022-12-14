/**
* @file Serialization.h
* @brief Template Operations for Serializing and Deserializing Data to and from Network Byte Order
* @authors Frank Reiser
* @date Created on November 14, 2022
*/

#ifndef REISERRT_BYTESTREAMBUF_SERIALIZATION_H
#define REISERRT_BYTESTREAMBUF_SERIALIZATION_H

#include "ByteStreamTypesFwd.h"

#include <iostream>
#include <endian.h>

namespace ReiserRT
{
    namespace Utility
    {

        /**
        * @brief Deserialize Network Ordered Bytes from a Basic Input Stream into a Type
        *
        * This helper template operation converts network ordered bytes from the byte stream into type T.
        *
        * @tparam T Type T is the type to convert to.
        * @param byteStream A reference to the byte stream containing the network ordered bytes.
        * @return The number of bytes deserialized which may be less than the size of T if the stream does not remain
        * in the "good" state for the entire operation.
        */
        template < typename T >
        size_t _deserializeFromByteStream( InputByteStream & byteStream, unsigned char * pType );

        /**
        * @brief Serialize a Type onto a Network Ordered Basic Output Stream.
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
        size_t _serializeToByteStream( OutputByteStream & byteStream, const unsigned char * pType );

        /**
        * @brief Convert Network Ordered Bytes from a Basic Input Stream into a Type
        *
        * This template operation converts network ordered bytes from the byte stream into return value type T.
        *
        * @tparam T Type T is the type to convert to and return. It must be a numeric or enumerator type.
        * @param byteStream A reference to the byte stream containing the network ordered bytes.
        * @return Returns value type T.
        * @warning The type may not be fully deserialized if the stream hits EOF before all bytes are deserialized.
        * The client should guard against this possibility by modifying the stream exception mask to throw an std::ios_base::failure
        * exception should std::ios_base::badbit or std::ios_base::eofbit become asserted.
        */
        template < typename T >
        T netToType( InputByteStream & byteStream )
        {
            union { unsigned char buf[ sizeof ( T ) ]; T t; } u;
            _deserializeFromByteStream< T >( byteStream, u.buf );
            return u.t;
        }

        /**
        * @brief Convert Network Ordered Bytes from Basic Input Stream into a Type
        *
        * This template operation converts network ordered bytes from the byte stream into type T via output
        * argument. It returns the number of bytes successfully read from the stream.
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
        size_t netToType( InputByteStream & byteStream, T & t )
        {
            return _deserializeFromByteStream< T >( byteStream, reinterpret_cast< unsigned char * >( &t ) );
        }

        /**
        * @brief Convert a Type onto a Network Ordered Output Stream.
        *
        * This template operation converts a type of type T onto a network ordered byte stream.
        *
        * @tparam T Type T is the type to convert from. It must be a numeric or enumerator type.
        * @param t The value to serialize.
        * @param byteStream A reference to the byte stream where the network ordered bytes will be written to.
        * @return The number of bytes serialized.
        */
        template < typename T >
        size_t typeToNet( const T & t, OutputByteStream & byteStream )
        {
            return _serializeToByteStream< T >( byteStream, reinterpret_cast< const unsigned char * >( &t ) );
        }


        /////////// Template Helper Operations Implementations Below ////////////

        template < typename T >
        size_t _deserializeFromByteStream( InputByteStream & byteStream, unsigned char * pType )
        {
            static_assert( std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_enum<T>::value,
                           "Type T must be an integer, floating point or enumerator type" );
            size_t i = 0;
#if ( __BYTE_ORDER == __BIG_ENDIAN )
            if ( byteStream )
            {
                for( ; sizeof ( T ) != i; ++i )
                {
                    byteStream.get( *pType++ );
                    if ( !byteStream ) break;
                }
            }
#elif ( __BYTE_ORDER == __LITTLE_ENDIAN )
            if ( byteStream )
            {
                pType += sizeof( T ) - 1;
                for( ; sizeof ( T ) != i; ++i )
                {
                    byteStream.get( *pType-- );
                    if ( !byteStream ) break;
                }
            }
#else
#error "Preprocessor symbol __BYTE_ORDER must be defined as __BIG_ENDIAN or __LITTLE_ENDIAN!!!"
#endif
            return i;
        }

        template < typename T >
        size_t _serializeToByteStream( OutputByteStream & byteStream, const unsigned char * pType )
        {
            static_assert( std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_enum<T>::value,
                           "Type T must be an integer, floating point or enumerator type" );
            size_t i = 0;
#if ( __BYTE_ORDER == __BIG_ENDIAN )
            if ( byteStream )
            {
                for ( ; sizeof( T ) != i; ++i )
                {
                    byteStream.put( *pType++ );
                    if ( !byteStream ) break;
                }
            }
#elif ( __BYTE_ORDER == __LITTLE_ENDIAN )
            if ( byteStream )
            {
                pType += sizeof( T ) - 1;
                for ( ; sizeof( T ) != i; ++i )
                {
                    byteStream.put( *pType-- );
                    if ( !byteStream ) break;
                }
            }
#else
#error "Preprocessor symbol __BYTE_ORDER must be defined as __BIG_ENDIAN or __LITTLE_ENDIAN!!!"
#endif
            return i;
        }

    }
}


#endif //REISERRT_BYTESTREAMBUF_SERIALIZATION_H
