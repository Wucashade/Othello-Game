# CMakeList.txt : CMake project for Othello-Game, include source and define
# project specific logic here.
#

# Add source to this project's executable.
cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable (Othello-Game "main.cpp" "Window.cpp" "Game.cpp")



target_link_libraries(Othello-Game ${SDL2_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Othello-Game PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
