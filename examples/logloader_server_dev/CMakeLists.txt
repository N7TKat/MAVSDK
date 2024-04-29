cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(logloader_server_dev)

add_executable(logloader_server_dev
    logloader_server_dev.cpp
)

find_package(MAVSDK REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(logloader_server_dev
    MAVSDK::mavsdk
    Threads::Threads
)

if(NOT MSVC)
    add_compile_options(logloader_server_dev PRIVATE -Wall -Wextra)
else()
    add_compile_options(logloader_server_dev PRIVATE -WX -W2)
endif()
