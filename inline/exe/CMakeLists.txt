
cmake_minimum_required(VERSION 3.1)
project(C_MAKE_DEMO)

if(APPLE)
    # no warning
    # https://stackoverflow.com/questions/31561309/cmake-warnings-under-os-x-macosx-rpath-is-not-specified-for-the-following-targe
    set(CMAKE_MACOSX_RPATH 0)
endif()

set(self_binary_name main)

set(Source_files 
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	)

add_executable(${self_binary_name} ${Source_files})

if (WIN32)
  # visual studio file encoding
  target_compile_options(${self_binary_name} PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
#elseif(APPLE)
else()
  target_link_libraries(${self_binary_name} dl) # dlopen  
endif()
if(APPLE)
endif()

# visual studio project tree
source_group(files FILES ${Source_files})


add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../lib build_lib)