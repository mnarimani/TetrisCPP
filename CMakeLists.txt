cmake_minimum_required(VERSION 3.15.3)
project(TetrisCPP)

set(CMAKE_CXX_STANDARD 14)

file(GLOB TetrisCPP_SRC
        "src/*.h"
        "src/*.cpp"
        )

add_executable(TetrisCPP ${TetrisCPP_SRC})