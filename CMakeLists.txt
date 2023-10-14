cmake_minimum_required(VERSION 3.0...3.5)

project(Terraria-2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(DEPENDENCIES_DIR_NAME dependencies)
set(DEPENDENCIES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${DEPENDENCIES_DIR_NAME})

add_subdirectory(VaKon2D)
add_subdirectory(game)

add_subdirectory(lib/texture-manager)