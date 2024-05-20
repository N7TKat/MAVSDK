cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mission_logloader_async_dev)

add_executable(mission_logloader_async_dev
mission_logloader_async_dev.cpp
)

find_package(MAVSDK REQUIRED)
find_package(nlohmann_json REQUIRED)
include_directories(${json_SOURCE_DIR}/include)

target_link_libraries(mission_logloader_async_dev
    MAVSDK::mavsdk
    nlohmann_json::nlohmann_json
)


if(NOT MSVC)
    add_compile_options(mission_logloader_async_dev PRIVATE -Wall -Wextra)
else()
    add_compile_options(mission_logloader_async_dev PRIVATE -WX -W2)
endif()
