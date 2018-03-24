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



add_executable(map
    ${CMAKE_CURRENT_LIST_DIR}/stdmap.cpp
)

if(WIN32)
	target_compile_options(map PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
endif()
    