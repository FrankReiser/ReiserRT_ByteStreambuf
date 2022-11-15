/**
* @file inputByteStreambufTest.cpp
* @brief Specification File for Test Data to Verify Input Serialization and ByteStreambuf
* @authors Frank Reiser
* @date Created on November 14, 2022
*/

#include "ByteStreambuf.h"
#include "Serialization.h"

#include "TestData.h"

using namespace ReiserRT::Utility;

int main()
{
    int retCode = 0;

    do {
        // Construct ByteStreambuf with data block we will be reading from. We will read from a test data block
        // with verifiable content.
        ByteStreambuf byteStreambuf{ testData, sizeof( testData ), std::ios::in };

        // Now construct an Input Byte Stream with address of the ByteStreambuf instance. (What, no reference?)
        InputByteStream inputByteStream{ &byteStreambuf };

        // How many bytes are in the stream buffer
        const auto availBytesForReading = inputByteStream.rdbuf()->in_avail();
        if ( sizeof( testData) != availBytesForReading )
        {
            std::cout << "Expected Input Stream read buffer would indicate that " << sizeof( testData )
                << " bytes are available for reading. Found "  << availBytesForReading
                << " are available" << std::endl;
            retCode = 1;
            break;
        }

        // Read bytes until Stream NOT OKAY.
        size_t i = 0;
        for ( ;; )
        {
            // Cast to unsigned char as get returns "int_type", even for char stream.
            const auto u = (unsigned char)inputByteStream.get();

            // Here, we will check the stream state each byte extracted. The better way would be to set
            // the exception mask, but we are not there yet.
            if ( !inputByteStream )
            {
                // If we have not read the required number of bytes before the stream went bad, then
                // we failed.
                if ( sizeof( testData ) < i )
                {
                    std::cout << "Stream NOT OKAY after byte extracted at iteration " << i << std::endl;
                    retCode = 2;
                }
                break;
            }

            if (u != testData[i])
            {
                std::cout << "Failed to read a correct byte value from input stream.  Expected 0x"
                     << std::hex << (unsigned int)testData[i] << ", got 0x" << std::hex << (unsigned int)u
                     << " @i=" << i << "\n";
                retCode = 3;
                break;
            }
            ++i;
        }
        if ( retCode ) break;

        // How many bytes are in the stream? We expect zero after reading the entire buffer
        if ( 0 != inputByteStream.rdbuf()->in_avail() )
        {
            std::cout << "Expected Input Stream read buffer would indicate that 0 bytes are available for reading!"
                << std::endl;
            retCode = 4;
            break;
        }

        // The stream should be in a FAILED state as we have read past the end of it above.
        // If it is Okay then this is an error
        if ( inputByteStream )
        {
            std::cout << "Expected Input Stream to be NOT OKAY after reading past end of stream buffer!"
                << std::endl;
            retCode = 5;
            break;
        }

        // Clear the stream errors and verify it is OKAY
        inputByteStream.clear();
        if ( !inputByteStream )
        {
            std::cout << "Expected Input Stream to be OKAY after clearing stream errors!"
                << std::endl;
            retCode = 6;
            break;
        }

        // Rewind the input stream and verify get position
        inputByteStream.seekg( 0 );
        if ( 0 != inputByteStream.tellg() )
        {
            std::cout << "Expected Input Stream to be get position to be reset to zero!"
                << std::endl;
            retCode = 7;
            break;
        }

        // TEST UNSIGNED SHORT
        // Note explicit template specification, just like static_cast<T>() syntax is required!!!!
        // The return type by itself won't do it!
        const auto uShortVal1 = netToType<unsigned short>( inputByteStream );
        if ( uShortVal1 != uShortTestVal1 )
        {
            std::cout << "netToType<unsigned short> FAILED!  Expected 0x" << std::hex << uShortTestVal1
                      << ", got 0x" << std::hex << uShortVal1
                      << std::endl;
            retCode = 8;
            break;
        }

        // How many bytes are in the stream. I expect sizeof( unsigned short ) less than buffer size.
        auto expectedBytesLeft = sizeof( testData ) - sizeof( unsigned short );
        if ( inputByteStream.rdbuf()->in_avail() != expectedBytesLeft )
        {
            std::cout << "Expected Input Stream buffer would have " << expectedBytesLeft
                << " remaining for input and " << inputByteStream.rdbuf()->in_avail() << " are remaining"
                << std::endl;
            retCode = 9;
            break;
        }

        // Without Rewinding, Read the next bytes as an unsigned short
        // This is the only test we will do without rewind. Once we have proven the stream
        // advances, we don't need to keep checking this.
        const auto uShortVal2 = netToType<unsigned short>( inputByteStream );
        if ( uShortVal2 != uShortTestVal2 )
        {
            std::cout << "netToType<unsigned short> FAILED!  Expected 0x" << std::hex << uShortTestVal2
                      << ", got 0x" << std::hex << uShortVal2
                      << std::endl;
            retCode = 10;
            break;
        }

        // How many bytes are in the stream. I expect sizeof( unsigned short ) * 2 less than buffer size.
        expectedBytesLeft = sizeof( testData ) - sizeof( unsigned short ) * 2;
        if ( inputByteStream.rdbuf()->in_avail() != expectedBytesLeft )
        {
            std::cout << "Expected Input Stream buffer would have " << expectedBytesLeft
                      << " remaining for input and " << inputByteStream.rdbuf()->in_avail() << " are remaining"
                      << std::endl;
            retCode = 11;
            break;
        }

        // TEST SIGNED SHORT
        inputByteStream.seekg( 0 );    // Rewind
        const auto sShortVal = netToType<signed short>( inputByteStream );
        if (sShortVal != sShortTestVal )
        {
            std::cout << "netToType<signed short> FAILED!  Expected 0x" << std::hex << (unsigned short)sShortTestVal
                      << ", got 0x" << std::hex << (unsigned short)sShortVal
                      << std::endl;
            retCode = 12;
            break;
        }

        // How many bytes are in the stream. I expect same as for the unsigned short
        expectedBytesLeft = sizeof( testData ) - sizeof( signed short );
        if ( expectedBytesLeft != inputByteStream.rdbuf()->in_avail() )
        {
            std::cout << "Expected Input Stream buffer would have " << expectedBytesLeft
                      << " remaining for input and " << inputByteStream.rdbuf()->in_avail() << " are remaining"
                      << std::endl;
            retCode = 13;
            break;
        }

        // TEST UNSIGNED INT
        inputByteStream.seekg( 0 );    // Rewind
        const auto uIntVal = netToType<unsigned int>( inputByteStream );
        if ( uIntVal != uIntTestVal )
        {
            std::cout << "netToType<unsigned int> FAILED!  Expected 0x" << std::hex << uIntTestVal
                      << ", got 0x" << std::hex << uIntVal
                      << std::endl;
            retCode = 14;
            break;
        }

        // How many bytes are in the stream. I expect sizeof( unsigned int ) less than buffer size.
        expectedBytesLeft = sizeof( testData ) - sizeof( unsigned int );
        if ( inputByteStream.rdbuf()->in_avail() != expectedBytesLeft )
        {
            std::cout << "Expected Input Stream buffer would have " << expectedBytesLeft
                      << " remaining for input and " << inputByteStream.rdbuf()->in_avail() << " are remaining"
                      << std::endl;
            retCode = 15;
            break;
        }

        // TEST SIGNED INT
        inputByteStream.seekg( 0 );    // Rewind
        const auto sIntVal = netToType<signed int>( inputByteStream );
        if ( sIntVal != sIntTestVal )
        {
            std::cout << "netToType<signed int> FAILED!  Expected 0x" << std::hex << (unsigned int)sIntTestVal
                      << ", got 0x" << std::hex << (unsigned int)sIntVal
                      << std::endl;
            retCode = 16;
            break;
        }

        // How many bytes are in the stream. I expect same as for the unsigned int
        if ( expectedBytesLeft != inputByteStream.rdbuf()->in_avail() )
        {
            std::cout << "Expected Input Stream buffer would have " << expectedBytesLeft
                      << " remaining for input and " << inputByteStream.rdbuf()->in_avail() << " are remaining"
                      << std::endl;
            retCode = 17;
            break;
        }

        // TEST UNSIGNED LONG
        inputByteStream.seekg( 0 );    // Rewind
        const auto uLongVal = netToType<unsigned long>( inputByteStream );
        if ( sIntVal != sIntTestVal )
        {
            std::cout << "netToType<unsigned long> FAILED!  Expected 0x" << std::hex << uLongTestVal
                      << ", got 0x" << std::hex << uLongVal
                      << std::endl;
            retCode = 18;
            break;
        }

        // How many bytes are in the stream. I expect sizeof( unsigned long ) less than buffer size.
        expectedBytesLeft = sizeof( testData ) - sizeof( unsigned long );
        if ( inputByteStream.rdbuf()->in_avail() != expectedBytesLeft )
        {
            std::cout << "Expected Input Stream buffer would have " << expectedBytesLeft
                      << " remaining for input and " << inputByteStream.rdbuf()->in_avail() << " are remaining"
                      << std::endl;
            retCode = 19;
            break;
        }

        // TEST SIGNED LONG
        inputByteStream.seekg( 0 );    // Rewind
        const auto sLongVal = netToType<signed long>( inputByteStream );
        if ( sLongVal != sLongTestVal )
        {
            std::cout << "netToType<unsigned long> FAILED!  Expected 0x" << std::hex
                      << (unsigned long)sLongTestVal
                      << ", got 0x" << std::hex << (unsigned long)sLongVal
                      << std::endl;
            retCode = 20;
            break;
        }

        // How many bytes are in the stream. I expect same as for the unsigned long
        if ( expectedBytesLeft != inputByteStream.rdbuf()->in_avail() )
        {
            std::cout << "Expected Input Stream buffer would have " << expectedBytesLeft
                      << " remaining for input and " << inputByteStream.rdbuf()->in_avail() << " are remaining"
                      << std::endl;
            retCode = 21;
            break;
        }

        // TEST FLOAT
        inputByteStream.seekg( 0 );    // Rewind
        const auto floatVal = netToType<float>( inputByteStream );
        if ( floatVal != floatTestVal )
        {
            std::cout << "netToType<unsigned long> FAILED!  Expected "
                      << floatTestVal
                      << ", got " << floatVal
                      << std::endl;
            retCode = 22;
            break;
        }

        // How many bytes are in the stream. I expect sizeof( float ) less than buffer size.
        expectedBytesLeft = sizeof( testData ) - sizeof( float );
        if ( inputByteStream.rdbuf()->in_avail() != expectedBytesLeft )
        {
            std::cout << "Expected Input Stream buffer would have " << expectedBytesLeft
                      << " remaining for input and " << inputByteStream.rdbuf()->in_avail() << " are remaining"
                      << std::endl;
            retCode = 23;
            break;
        }

        // TEST DOUBLE
        inputByteStream.seekg( 0 );    // Rewind
        const auto doubleVal = netToType<double>( inputByteStream );
        if ( doubleVal != doubleTestVal )
        {
            std::cout << "netToType<unsigned long> FAILED!  Expected "
                      << doubleTestVal
                      << ", got " << doubleVal
                      << std::endl;
            retCode = 24;
            break;
        }

        // How many bytes are in the stream. I expect sizeof( double ) less than buffer size.
        expectedBytesLeft = sizeof( testData ) - sizeof( double );
        if ( inputByteStream.rdbuf()->in_avail() != expectedBytesLeft )
        {
            std::cout << "Expected Input Stream buffer would have " << expectedBytesLeft
                      << " remaining for input and " << inputByteStream.rdbuf()->in_avail() << " are remaining"
                      << std::endl;
            retCode = 25;
            break;
        }

        // TEST WITH EXCEPTIONS ENABLED

        // Capture Start Conditions. It is often appropriate to return them to their original state
        // afterwards. However, if you own the object, you should be able to set it once and forget it,
        const auto startExceptionState = inputByteStream.exceptions();

        // Or in 'badbit' and 'failbit' bits. This should result on an exception being thrown if we read past the end.
        inputByteStream.exceptions( startExceptionState | ( std::ios_base::badbit | std::ios_base::failbit ) );

        // Try this as we should eventually throw.
        try
        {
            // We should already be at the end from the previous test. But just incase things change, we will make sure.
            // We will read the 'expectedBytesLeft' and then one more.
            for ( ; 0 != expectedBytesLeft; --expectedBytesLeft )
                inputByteStream.get();

            // This should definitely throw
            inputByteStream.get();

            // If we find ourselves here, we failed.
            std::cout << "Excepted Exception to be thrown and that did not occur!" << std::endl;
            retCode = 26;
            break;
        }
        catch ( const std::ios_base::failure & e )
        {
//            std::cout << "Exception caught!" << std::endl;

            // If 'expectedBytesLeft' is NOT zero then we FAILED this test.
            if ( 0 != expectedBytesLeft )
            {
                retCode = 27;
                break;
            }
        }
        
    } while( false);


    return retCode;
}
