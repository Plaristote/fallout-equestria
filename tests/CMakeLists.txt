cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

project(FalloutEquestria)

include_directories(../include ../angelscript src)

if(UNIX)
  if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "-std=c++0x -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNIT_TESTER")
  endif()
endif()

#file(GLOB_RECURSE sourceFiles
#     ../include/*.h
#     ../include/*.hpp
#     ../src/timer.cpp
#     ../src/statsheet.cpp
#     ../src/serializer.cpp
#     ../src/world.cpp
#     ../src/data.cpp
#     ../src/json.cpp
#     ../src/directory.cpp
#     ../src/Boots/semaphore.cpp
#     ../src/strings.cpp
#     ../src/level/waypointmodifier.cpp
#     ../src/scriptengine.cpp
#     ../src/gameui.cpp
#     ../src/rocket_extension.cpp
#     src/*.cpp)

file(GLOB_RECURSE sourceFiles
     ../angelscript/*.cpp
     ../src/*.cpp
     src/*.cpp)

add_executable(testing ${sourceFiles})

if (UNIX)
 target_link_libraries(testing -L/usr/lib/panda3d p3framework panda pandafx pandaphysics pandaexpress p3dtoolconfig p3dtool p3direct pandaegg                                                                      p3rocket RocketCore RocketControls
                                                  angelscript pthread)
endif()
