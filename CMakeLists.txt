cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

project(FalloutEquestria)

cmake_policy(SET CMP0015 NEW) # Linked directories are relative to the CMakeLists.txt file

include_directories(code/game/include
                    code/world/include
                    code/utils/include
                    code/angelscript
                    3rd_parties/include)
link_directories(3rd_parties/lib)

if(UNIX)
  if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wno-deprecated-declarations -pedantic")
  endif()
endif()

file(GLOB_RECURSE sourceFiles
     # Game
     code/game/include/*.h
     code/game/include/*.hpp
     code/game/src/*.cpp
     # World
     code/world/include/*.h
     code/world/include/*.hpp
     code/world/src/*.cpp
     # Utils
     code/utils/include/*.h
     code/utils/include/*.hpp
     code/utils/src/*.cpp
     # Angelscript
     code/angelscript/*.cpp)

if(UNIX)
  add_executable(game ${sourceFiles})
else()
  add_executable(game ${sourceFiles} ressource.rc)
endif()

if(UNIX)
  target_link_libraries(game -L/usr/lib/panda3d p3framework panda pandafx pandaphysics pandaexpress p3dtoolconfig p3dtool p3direct pandaegg
                                              p3rocket RocketCore RocketControls
                                              angelscript pthread z)
else()
  target_link_libraries(game libp3framework libpanda libpandafx libpandaphysics libpandaexpress libp3dtoolconfig libp3dtool libp3direct libpandaegg libp3rocket
                        RocketCore RocketControls
                        angelscript zlibstatic)
endif()
