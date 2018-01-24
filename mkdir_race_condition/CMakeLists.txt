
cmake_minimum_required(VERSION 3.1)
project(C_MAKE_DEMO)


set(self_binary_name test_mkdir_race_condition)

set(Source_files 
	${CMAKE_CURRENT_SOURCE_DIR}/mymkdir.c
	${CMAKE_CURRENT_SOURCE_DIR}/mymkdir.h
	${CMAKE_CURRENT_SOURCE_DIR}/mymkdir_tests.c
	)

add_executable(${self_binary_name} ${Source_files})

if (WIN32)
  # visual sutdio file encoding
  target_compile_options(${self_binary_name} PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
#elseif(APPLE)
else()
  # fPIC for other library link
  target_compile_options(${self_binary_name} PRIVATE -fPIC)
  target_compile_options(${self_binary_name} PRIVATE -fvisibility=hidden)
  target_compile_options(${self_binary_name} PRIVATE -fno-common )
endif()


source_group(files FILES ${Source_files})
