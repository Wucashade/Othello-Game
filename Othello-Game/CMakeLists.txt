﻿# CMakeList.txt : CMake project for Othello-Game, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Othello-Game "Othello-Game.cpp" "Othello-Game.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Othello-Game PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.