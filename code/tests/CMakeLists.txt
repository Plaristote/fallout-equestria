cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

project(FalloutEquestria)

include_directories(../game/include ../world/include ../angelscript src)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "-std=c++0x -Wno-deprecated-declarations")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNIT_TESTER")
endif()

file(GLOB_RECURSE sourceFiles
     ../angelscript/*.cpp
     ../game/src/*.cpp
     ../world/src/*.cpp
     src/*.cpp)

add_executable(testing ${sourceFiles})

if (UNIX)
 target_link_libraries(testing -L/usr/lib/panda3d p3framework panda pandafx pandaphysics pandaexpress p3dtoolconfig p3dtool p3direct pandaegg                                                                      p3rocket RocketCore RocketControls
                                                  angelscript pthread)
endif()
