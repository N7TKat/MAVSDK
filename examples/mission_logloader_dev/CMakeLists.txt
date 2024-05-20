cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mission_logloader_dev)

add_executable(mission_logloader_dev
mission_logloader_dev.cpp
)

find_package(MAVSDK REQUIRED)

target_link_libraries(mission_logloader_dev
    MAVSDK::mavsdk
)

if(NOT MSVC)
    add_compile_options(mission_logloader_dev PRIVATE -Wall -Wextra)
else()
    add_compile_options(mission_logloader_dev PRIVATE -WX -W2)
endif()
