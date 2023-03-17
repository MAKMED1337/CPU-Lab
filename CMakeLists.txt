cmake_minimum_required(VERSION 3.24)
project(lab)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab main.cpp ASM_Compiler.cpp Processor.cpp)
target_link_libraries(lab PRIVATE "-Wl,--stack,536870912")
