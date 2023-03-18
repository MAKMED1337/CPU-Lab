cmake_minimum_required(VERSION 3.24)
project(lab)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab main.cpp ASM/Compiler.cpp Processor.cpp ASM/InstructionList.cpp ASM/ParseStream.cpp ASM/Instructions.cpp)

add_definitions(-DFMT_HEADER_ONLY)
find_package(fmt REQUIRED)
target_link_libraries(lab PRIVATE fmt::fmt)

target_link_libraries(lab PRIVATE "-Wl,--stack,536870912")