cmake_minimum_required(VERSION 3.24)
project(lab)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab main.cpp ASM/Compiler.cpp Processor.cpp ASM/InstructionList.cpp ASM/ParseStream.cpp)
target_link_libraries(lab PRIVATE "-Wl,--stack,536870912")
