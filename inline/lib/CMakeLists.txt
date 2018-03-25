
cmake_minimum_required(VERSION 3.1)
project(C_LIBRARY_DEMO)

if(APPLE)
    # no warning
    # https://stackoverflow.com/questions/31561309/cmake-warnings-under-os-x-macosx-rpath-is-not-specified-for-the-following-targe
    set(CMAKE_MACOSX_RPATH 0)
endif()


set(self_library_name inline_test)

set(Source_files 
	${CMAKE_CURRENT_LIST_DIR}/header.h
  ${CMAKE_CURRENT_LIST_DIR}/implement.cpp
	)

add_library(${self_library_name} SHARED  ${Source_files})

if (WIN32)
  # visual studio file encoding
  target_compile_options(${self_library_name} PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
endif()

source_group(files FILES ${Source_files})

