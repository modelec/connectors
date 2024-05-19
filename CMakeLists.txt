cmake_minimum_required(VERSION 3.25)
project(connectors)

set(CMAKE_CXX_STANDARD 17)

find_package(Modelec COMPONENTS
        TCPClient
        Utils
        REQUIRED
)

add_executable(connectors
        serialib.cpp
        MyTCPClient.cpp
        MyTCPClient.h
        main.cpp
)

target_link_libraries(connectors Modelec::TCPClient Modelec::Utils)