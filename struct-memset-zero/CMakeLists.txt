cmake_minimum_required(VERSION 3.1)

project(tests)

if(APPLE)
    # no warning
    # https://stackoverflow.com/questions/31561309/cmake-warnings-under-os-x-macosx-rpath-is-not-specified-for-the-following-targe
    set(CMAKE_MACOSX_RPATH 0)
endif()


foreach(i ${CMAKE_CXX_COMPILE_FEATURES})
  message(STATUS "compiler support ${i}")
endforeach()



add_executable(struct-memset-zero
    ${CMAKE_CURRENT_LIST_DIR}/struct-memset-zero.c
)

if(WIN32)
	target_compile_options(struct-memset-zero PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
endif()
    