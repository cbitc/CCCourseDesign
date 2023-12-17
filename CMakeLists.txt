cmake_minimum_required(VERSION 3.15)
project(MyGame)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
file(GLOB SRC CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(main ${SRC})

