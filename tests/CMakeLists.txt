add_executable( inputByteStreambufTest "" )
target_sources( inputByteStreambufTest PRIVATE inputByteStreambufTest.cpp TestData.cpp)
target_include_directories( inputByteStreambufTest PUBLIC ../src )
target_link_libraries( inputByteStreambufTest ReiserRT_ByteStreambuf  )
add_test( NAME runInputByteStreambufTest COMMAND $<TARGET_FILE:inputByteStreambufTest> )

