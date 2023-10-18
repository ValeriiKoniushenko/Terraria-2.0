cmake_minimum_required(VERSION 2.6...3.5)

project (SimplexNoise)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_library(Simplex-Noise src/SimplexNoise.cpp src/SimplexNoise.h)
target_include_directories(Simplex-Noise PUBLIC src/)