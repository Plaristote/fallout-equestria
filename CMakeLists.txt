cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE Debug)

project(FalloutEquestria)

include_directories(include)

set(CMAKE_CXX_FLAGS "-std=c++0x -Wno-deprecated-declarations")

file(GLOB_RECURSE sourceFiles
     src/*.cpp)

add_executable(game ${sourceFiles})

target_link_libraries(game -L/usr/lib/panda3d p3framework panda pandafx pandaexpress p3dtoolconfig p3dtool p3pystub p3direct p3rocket RocketCore RocketControls)
