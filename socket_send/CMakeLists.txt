

cmake_minimum_required(VERSION 3.1)
project(C_MAKE_DEMO)

if(APPLE)
    set(CMAKE_MACOSX_RPATH 0)
endif()


set(self_server udp_server)
add_executable(${self_server} udp_server.c)

set(self_client udp_client)
add_executable(${self_client} udp_client.c)

if (WIN32)
  # visual studio file encoding
  target_compile_options(${self_server} PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
  target_compile_options(${self_client} PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
#elseif(APPLE)
else()
  # fPIC for other library link
  target_compile_options(${self_server} PRIVATE -fPIC)
  target_compile_options(${self_server} PRIVATE -fvisibility=hidden)
  target_compile_options(${self_server} PRIVATE -fno-common )
  target_compile_options(${self_client} PRIVATE -fPIC)
  target_compile_options(${self_client} PRIVATE -fvisibility=hidden)
  target_compile_options(${self_client} PRIVATE -fno-common )
endif()
if(APPLE)
endif()

# visual studio project tree