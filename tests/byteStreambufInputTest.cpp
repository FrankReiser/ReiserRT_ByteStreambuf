/**
* @file byteStreambufInputTest.cpp
* @brief Specification File for Test Data to Verify Serialization and ByteStreambuf
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

        // How many bytes are in the stream. I expect zero now
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
        auto uShortVal = netToType<unsigned short>(inputByteStream );
        if ( uShortVal != 0x4241 )
        {
            std::cout << "netToType<unsigned short> FAILED!  Expected 0x4241, got 0x" << std::hex << uShortVal
                << std::endl;
            retCode = 8;
            break;
        }

        // How many bytes are in the stream. I expect 6 now
        if ( sizeof( testData ) - sizeof ( unsigned short ) != inputByteStream.rdbuf()->in_avail() )
        {
            std::cout << "Expected Input Stream read buffer would indicate that 4 bytes are available for reading!"
                << std::endl;
            retCode = 9;
            break;
        }

        // Rewind and
        inputByteStream.seekg( 0 );

        // TEST SIGNED SHORT
        auto iShortVal = netToType<signed short>(inputByteStream );
        if ( iShortVal != (signed short )0x4241 )
        {
            std::cout << "netToType< signed short> FAILED!  Expected 0x4241, got 0x" << std::hex << (unsigned short)iShortVal
                << std::endl;
            retCode = 10;
            break;
        }



    } while( false);


    return retCode;
}
