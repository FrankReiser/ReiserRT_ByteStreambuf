add_executable( inputByteStreambufTest "" )
target_sources( inputByteStreambufTest PRIVATE inputByteStreambufTest.cpp TestData.cpp)
target_include_directories( inputByteStreambufTest PUBLIC ../src )
target_link_libraries( inputByteStreambufTest ReiserRT_ByteStreambuf  )
target_compile_options( inputByteStreambufTest PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
add_test( NAME runInputByteStreambufTest COMMAND $<TARGET_FILE:inputByteStreambufTest> )

add_executable( outputByteStreambufTest "" )
target_sources( outputByteStreambufTest PRIVATE outputByteStreambufTest.cpp TestData.cpp)
target_include_directories( outputByteStreambufTest PUBLIC ../src )
target_link_libraries( outputByteStreambufTest ReiserRT_ByteStreambuf  )
target_compile_options( outputByteStreambufTest PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
add_test( NAME runOutputByteStreambufTest COMMAND $<TARGET_FILE:outputByteStreambufTest> )
