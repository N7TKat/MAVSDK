cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(sd_download_dev)

add_executable(sd_download_dev
    sd_download_dev.cpp
)

find_package(MAVSDK REQUIRED)

target_link_libraries(sd_download_dev
    MAVSDK::mavsdk
)

if(NOT MSVC)
    add_compile_options(sd_download_dev PRIVATE -Wall -Wextra)
else()
    add_compile_options(sd_download_dev PRIVATE -WX -W2)
endif()
