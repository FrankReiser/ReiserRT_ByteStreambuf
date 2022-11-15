/**
* @file outputByteStreambufTest.cpp
* @brief Test Harness to Verify Output Serialization and ByteStreambuf
* @authors Frank Reiser
* @date Created on November 14, 2022
*/

#include "ByteStreambuf.h"
#include "Serialization.h"

#include "TestData.h"

using namespace ReiserRT::Utility;

size_t compareBytes( const unsigned char * pBytesUnderTest, const unsigned char * pGoldStandard, size_t numBytes )
{
    size_t index = 0;
    for ( ; numBytes != index; ++index )
    {
        if ( *pBytesUnderTest++ != *pGoldStandard++ )
            break;
    }

    return index;
}

int main()
{
    unsigned char outputBuffer[8];
    int retCode = 0;

    do {
        // Construct ByteStreambuf with data block we will be writing to.
        ByteStreambuf byteStreambuf{ outputBuffer, sizeof( outputBuffer ), std::ios::out };

        // Construct and output stream using our stream buffer.
        OutputByteStream outputByteStream( &byteStreambuf );

        // We will verify the number of bytes written on subsequent tests.
        size_t bytesWritten = 0;

        // TEST UNSIGNED SHORT #1 and #2
        bytesWritten = typeToNet(uShortTestVal1, outputByteStream );
        if ( sizeof ( uShortTestVal1 ) != bytesWritten )
        {
            std::cout << "Expected Output Stream write would indicate that " << sizeof( uShortTestVal1 )
                      << " bytes were written. Found "  << bytesWritten
                      << " were written" << std::endl;
            retCode = 1;
            break;
        }

        // There should be room to write another into the buffer without rewinding.
        // We want to verify that we are positioned appropriately in the stream.
        bytesWritten += typeToNet( uShortTestVal2, outputByteStream );
        if ( sizeof ( uShortTestVal1 ) * 2 != bytesWritten )
        {
            std::cout << "Expected Output Stream write would indicate that " << sizeof( uShortTestVal1 ) * 2
                      << " bytes were written. Found "  << bytesWritten
                      << " were written" << std::endl;
            retCode = 2;
            break;
        }

        // Now compare our buffer to the test data buffer for both ushort values we wrote.
        auto equivalentByteCount = compareBytes( outputBuffer, testData, bytesWritten );
        if ( equivalentByteCount != bytesWritten )
        {
            std::cout << "Failed to compare bytes at offset " << equivalentByteCount << ". Expected return of "
                << bytesWritten << std::endl;
            retCode = 3;
            break;
        }

        // TEST SIGNED SHORT
        outputByteStream.seekp( 0 );    // Rewind stream put position
        bytesWritten = typeToNet( sShortTestVal, outputByteStream );
        if ( sizeof ( sShortTestVal ) != bytesWritten )
        {
            std::cout << "Expected Output Stream write would indicate that " << sizeof( sShortTestVal )
                      << " bytes were written. Found "  << bytesWritten
                      << " were written" << std::endl;
            retCode = 4;
            break;
        }

        // Now compare our buffer to the test data buffer for the value we wrote.
        equivalentByteCount = compareBytes( outputBuffer, testData, bytesWritten );
        if ( equivalentByteCount != bytesWritten )
        {
            std::cout << "Failed to compare bytes at offset " << equivalentByteCount << ". Expected return of "
                      << bytesWritten << std::endl;
            retCode = 5;
            break;
        }

        // TEST UNSIGNED INT
        outputByteStream.seekp( 0 );    // Rewind stream put position
        bytesWritten = typeToNet( uIntTestVal, outputByteStream );
        if ( sizeof ( uIntTestVal ) != bytesWritten )
        {
            std::cout << "Expected Output Stream write would indicate that " << sizeof( uIntTestVal )
                      << " bytes were written. Found "  << bytesWritten
                      << " were written" << std::endl;
            retCode = 6;
            break;
        }

        // Now compare our buffer to the test data buffer for the value we wrote.
        equivalentByteCount = compareBytes( outputBuffer, testData, bytesWritten );
        if ( equivalentByteCount != bytesWritten )
        {
            std::cout << "Failed to compare bytes at offset " << equivalentByteCount << ". Expected return of "
                      << bytesWritten << std::endl;
            retCode = 7;
            break;
        }

        // TEST SIGNED INT
        outputByteStream.seekp( 0 );    // Rewind stream put position
        bytesWritten = typeToNet( sIntTestVal, outputByteStream );
        if ( sizeof ( sIntTestVal ) != bytesWritten )
        {
            std::cout << "Expected Output Stream write would indicate that " << sizeof( sIntTestVal )
                      << " bytes were written. Found "  << bytesWritten
                      << " were written" << std::endl;
            retCode = 8;
            break;
        }

        // Now compare our buffer to the test data buffer for the value we wrote.
        equivalentByteCount = compareBytes( outputBuffer, testData, bytesWritten );
        if ( equivalentByteCount != bytesWritten )
        {
            std::cout << "Failed to compare bytes at offset " << equivalentByteCount << ". Expected return of "
                      << bytesWritten << std::endl;
            retCode = 9;
            break;
        }

        // TEST UNSIGNED LONG
        outputByteStream.seekp( 0 );    // Rewind stream put position
        bytesWritten = typeToNet( uLongTestVal, outputByteStream );
        if ( sizeof ( uLongTestVal ) != bytesWritten )
        {
            std::cout << "Expected Output Stream write would indicate that " << sizeof( uLongTestVal )
                      << " bytes were written. Found "  << bytesWritten
                      << " were written" << std::endl;
            retCode = 10;
            break;
        }

        // Now compare our buffer to the test data buffer for the value we wrote.
        equivalentByteCount = compareBytes( outputBuffer, testData, bytesWritten );
        if ( equivalentByteCount != bytesWritten )
        {
            std::cout << "Failed to compare bytes at offset " << equivalentByteCount << ". Expected return of "
                      << bytesWritten << std::endl;
            retCode = 11;
            break;
        }

        // TEST SIGNED LONG
        outputByteStream.seekp( 0 );    // Rewind stream put position
        bytesWritten = typeToNet( sLongTestVal, outputByteStream );
        if ( sizeof ( sLongTestVal ) != bytesWritten )
        {
            std::cout << "Expected Output Stream write would indicate that " << sizeof( sLongTestVal )
                      << " bytes were written. Found "  << bytesWritten
                      << " were written" << std::endl;
            retCode = 12;
            break;
        }

        // Now compare our buffer to the test data buffer for the value we wrote.
        equivalentByteCount = compareBytes( outputBuffer, testData, bytesWritten );
        if ( equivalentByteCount != bytesWritten )
        {
            std::cout << "Failed to compare bytes at offset " << equivalentByteCount << ". Expected return of "
                      << bytesWritten << std::endl;
            retCode = 13;
            break;
        }

        // While we are here, we want to verify stream state since we are at a full buffer condition.
        // The stream should still be GOOD as we have not overflowed it yet.
        if ( !outputByteStream )
        {
            std::cout << "Expected Stream to be in a 'GOOD' state and it is not" << std::endl;
            retCode = 14;
            break;
        }

        // Attempt to overwrite the end
        bytesWritten = typeToNet( uShortTestVal1, outputByteStream );
        if ( 0 != bytesWritten )
        {
            std::cout << "Expected Stream to not except any more bytes and was still able to write "
            << bytesWritten << " bytes." << std::endl;
            retCode = 15;
            break;
        }

        // TEST STREAM SHOULD NOT BE OKAY
        if ( outputByteStream )
        {
            std::cout << "Expected Output Stream to be in a 'BAD' state and it is not!" << std::endl;
            retCode = 16;
            break;
        }

        // Clear the stream errors and verify it is OKAY
        outputByteStream.clear();
        if ( !outputByteStream )
        {
            std::cout << "Expected Output Stream to be 'GOOD' after clearing stream errors and it is not!"
                      << std::endl;
            retCode = 17;
            break;
        }

        // TEST FLOAT
        outputByteStream.seekp( 0 );    // Rewind stream put position
        bytesWritten = typeToNet( floatTestVal, outputByteStream );
        if ( sizeof ( floatTestVal ) != bytesWritten )
        {
            std::cout << "Expected Output Stream write would indicate that " << sizeof( floatTestVal )
                      << " bytes were written. Found "  << bytesWritten
                      << " were written" << std::endl;
            retCode = 18;
            break;
        }

        // Now compare our buffer to the test data buffer for the value we wrote.
        equivalentByteCount = compareBytes( outputBuffer, testData, bytesWritten );
        if ( equivalentByteCount != bytesWritten )
        {
            std::cout << "Failed to compare bytes at offset " << equivalentByteCount << ". Expected return of "
                      << bytesWritten << std::endl;
            retCode = 19;
            break;
        }

        // TEST DOUBLE
        outputByteStream.seekp( 0 );    // Rewind stream put position
        bytesWritten = typeToNet( doubleTestVal, outputByteStream );
        if ( sizeof ( doubleTestVal ) != bytesWritten )
        {
            std::cout << "Expected Output Stream write would indicate that " << sizeof( doubleTestVal )
                      << " bytes were written. Found "  << bytesWritten
                      << " were written" << std::endl;
            retCode = 20;
            break;
        }

        // Now compare our buffer to the test data buffer for the value we wrote.
        equivalentByteCount = compareBytes( outputBuffer, testData, bytesWritten );
        if ( equivalentByteCount != bytesWritten )
        {
            std::cout << "Failed to compare bytes at offset " << equivalentByteCount << ". Expected return of "
                      << bytesWritten << std::endl;
            retCode = 21;
            break;
        }

        // TEST WITH EXCEPTIONS ENABLED
        // Capture Start Conditions. It is often appropriate to return them to their original state
        // afterwards. However, if you own the object, you should be able to set it once and forget it,
        const auto startExceptionState = outputByteStream.exceptions();

        // Or in 'badbit' and 'failbit' bits. This should result on an exception being thrown if we write past the end.
        outputByteStream.exceptions( startExceptionState | ( std::ios_base::badbit | std::ios_base::failbit ) );

        // The stream should be at the end from the previous test. We should not be able to write a single byte without
        // throwing an exception
        try
        {
            unsigned char byte = 0;
            outputByteStream.put( byte );

            // If we find ourselves here, we failed.
            std::cout << "Excepted Exception to be thrown and that did not occur!" << std::endl;
            retCode = 22;
            break;
        }
        catch ( const std::ios_base::failure & e )
        {
//            std::cout << "Exception caught!" << std::endl;
        }

    } while (false);

    return retCode;
}
