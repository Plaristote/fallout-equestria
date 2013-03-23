cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

project(FalloutEquestria)

include_directories(include angelscript)

if(UNIX)
  if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wno-deprecated-declarations -pedantic --coverage")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAUDIO_BACKEND_SFML")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAUDIO_BACKEND_PANDA3D")
  endif()
endif()

file(GLOB_RECURSE sourceFiles
     include/*.h
     include/*.hpp
     src/*.cpp
     angelscript/*.cpp)

add_executable(game ${sourceFiles})

if (UNIX)
  target_link_libraries(game -L/usr/lib/panda3d p3framework panda pandafx pandaexpress p3dtoolconfig p3dtool p3direct
                                              p3rocket RocketCore RocketControls
                                              angelscript sfml-audio pthread)
else()
  target_link_libraries(game libp3framework libpanda libpandafx libpandaexpress libp3dtoolconfig libp3dtool libp3direct libp3rocket RocketCore RocketControls angelscript sfml-audio)
endif()
