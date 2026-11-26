cmake_minimum_required(VERSION 3.20)

project(CHIP8Emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chip8
    src/main.cpp
    src/Chip8.cpp
)

target_include_directories(chip8 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)