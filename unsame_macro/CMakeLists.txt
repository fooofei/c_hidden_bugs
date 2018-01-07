
cmake_minimum_required(VERSION 2.8.14)
project(unsame_macro)


set(BINARY_NAME unsame_error)
set(STATIC_LIBRARY_NAME mylib)


add_executable(${BINARY_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.c)


add_library(${STATIC_LIBRARY_NAME} STATIC 
 ${CMAKE_CURRENT_SOURCE_DIR}/a.c
 ${CMAKE_CURRENT_SOURCE_DIR}/a.h
 
 )

target_link_libraries(${BINARY_NAME} ${STATIC_LIBRARY_NAME})


if (APPLE)
    set(CMAKE_MACOSX_RPATH 0)
endif ()
if(WIN32)
 # no utf-8 warning in Visual Studio https://github.com/fooofei/cpp_notes/blob/master/visual_sutdio_warning_C4819_utf-8.md
 target_compile_options(${BINARY_NAME} PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
endif()

if (NOT WIN32)
    # fPIC for other library link
    target_compile_options(${STATIC_LIBRARY_NAME} PRIVATE -fPIC)
endif()


# here, you can see CMake is smarter.
# CMake have PRIVATE keyword, and PUBLIC keyword.
#   the PUBLIC is other module which include the staic library also
#   have the defines. 
target_compile_definitions(${STATIC_LIBRARY_NAME} PRIVATE "_USE_DATA2")

# solution
#target_compile_definitions(${STATIC_LIBRARY_NAME} PUBLIC "_USE_DATA2")
