# CMakeList.txt : CMake project for Othello-Game, include source and define
# project specific logic here.
#

# Add source to this project's executable.
cmake_minimum_required(VERSION 3.0)

set(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_library(SDL2_LIBRARY NAME SDL2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

message ([STATUS] "SDL2_INCLUDE_DIRS" ${SDL2_INCLUDE_DIR})
message ([STATUS] "SDL2_LIBRARIES" ${SDL2_LIBRARY})
message ([STATUS] "SDL2_IMAGE_INCLUDE_DIR" ${SDL2_IMAGE_INCLUDE_DIR})
message ([STATUS] "SDL2_TTF_INCLUDE_DIR" ${SDL2_TTF_INCLUDE_DIR})

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TFF_INCLUDE_DIRS})

file(COPY "disctextures" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

add_executable (Othello-Game "main.cpp" "Window.cpp" "Board.cpp" "Game.cpp" "Player.cpp")


target_link_libraries(Othello-Game ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Othello-Game PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
