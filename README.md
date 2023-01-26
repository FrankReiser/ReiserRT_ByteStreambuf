# ReiserRT_ByteStreambuf

Frank Reiser's C++11 implementation of a Byte Stream Buffer to perform serialization using iostream instances.
This component has been tested to be interface-able with C++20 compiles. Note that the compiled library code
is built using the c++11 standard.

## Overview
This library package affords the ability to use iostream instances with unsigned character blocks
of user provided memory for an iostream buffer. This is intended to be used for serializing and deserializing
raw data to or from a binary network transport. It is also intended to be used with 'un-formatted' I/O.
This means using the 'put' and 'get' stream operations as opposed to using the 'insertion' or 'extraction'
operators which do formatted I/O.

This package also provides `typeToNet` and `netToType` template code to perform serialization/deserialization
operations on iostream instances.

Class, `ByteStreambuf` derives from `std::basic_streambuf< unsigned char >` and overrides only what
is necessary to accomplish the job of un-formatted I/O on unsigned char data blocks. Once instantiated,
a `std::basic_istream< unsigned char >`, `std::basic_ostream< unsigned char >` or `std::basic_iostream< unsigned char >`
may be instantiated and utilized as follows (input example given):
  ```
  // Construct a ByteStrembuf with existing unsigned char data block.
  unsigned char byteBlock[ 128 ]; 
  ByteStreambuf byteStreambuf{ byteBlock, sizeof( byteBlock ), std::ios::in  };
  
  // Construct an InputByteStream object (alias for std::basic_istream< unsigned char>), associating
  // our instance of ByteStreambuf
  InputByteStream inputByteStream{ &byteStreambuf };

  // Read something from it
  const auto uShortVal = netToType< unsigned short >( inputByteStream );
  ```

You can do most things you would expect to be able to do with an iostream. As with iostream by default,
you have to do your own monitoring of stream state. Running off the end of the buffer
will leave the stream in a 'bad' state. Achieving this state alone, will not throw an exception.
If you require an exception be thrown, you may modify the stream's exception mask so that, exceptions will
be thrown on failure. Enabling exceptions alleviates the constant monitoring of stream state.
To utilize exception handling. Do the following:

  ```
  // Capture Start Exception Conditions. It is often appropriate to return them to their original state
  // afterwards. However, if you own the object, you should be able to set it once and forget it,
  const auto startExceptionState = inputByteStream.exceptions();
  
  // Or in 'badbit' and 'failbit' bits. This should result on an exception being thrown if we read past the end.
  inputByteStream.exceptions( startExceptionState | ( std::ios_base::badbit | std::ios_base::failbit ) );

  // Try I/O
  try
  {
      // Read something from it
      const auto uShortVal = netToType< unsigned short >( inputByteStream );
  }
  catch ( const std::ios_base::failure & e )
  {
     // Handle exception if caught.
  }
  ```

Output is similar to input. See various test and example code for possible use cases.

## Building and Installation
Roughly as follows:
1) Obtain a copy of the project
2) Create a build folder within the project root folder.
3) Switch directory to the build folder and run the following
   to configure and build the project for your platform:
   ```
   cmake ..
   cmake --build .
   ```
4) Test the library
   ```
   ctest
   ```
5) Install the library as follows (You'll most likely
   need root permissions to do this):
   ```
   sudo cmake --install .
   ```
   