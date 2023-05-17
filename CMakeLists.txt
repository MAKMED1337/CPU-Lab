cmake_minimum_required(VERSION 3.24)
project(lab)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab main.cpp ASM/Compiler.cpp Hardware/Processor.cpp ASM/InstructionList.cpp ASM/ParseStream.cpp ASM/Instructions.cpp Hardware/Code.cpp Hardware/Bus.cpp Hardware/IO.cpp Hardware/Display.cpp Hardware/Computer.cpp ASM/CodeSegment.hpp ASM/Loader.cpp Hardware/InteractiveCode.cpp Hardware/InteractiveCode.hpp)

add_definitions(-DFMT_HEADER_ONLY)
find_package(fmt REQUIRED)
target_link_libraries(lab PRIVATE fmt::fmt)

target_link_libraries(lab PRIVATE "-Wl,--stack,536870912")