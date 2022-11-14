add_executable( byteStreambufInputTest "" )
target_sources( byteStreambufInputTest PRIVATE byteStreambufInputTest.cpp TestData.cpp)
target_include_directories( byteStreambufInputTest PUBLIC ../src )
target_link_libraries( byteStreambufInputTest ReiserRT_ByteStreambuf  )
add_test( NAME runByteStreambufInputTest COMMAND $<TARGET_FILE:byteStreambufInputTest> )

