cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(max_flow) # Название проекта

add_executable(max_flow main.cpp)

target_include_directories(max_flow PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
