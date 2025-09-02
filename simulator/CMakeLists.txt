cmake_minimum_required(VERSION 3.16)
project(CANtrol VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES "src/*.c")

add_executable(CANtrol ${SRC_FILES})
target_include_directories(CANtrol PRIVATE include)
