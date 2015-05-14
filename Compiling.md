# Installing the dependencies #

Download and compile the dependencies from the following links.

**Panda3D 1.8.0 / 1.8.x CVS**

http://www.panda3d.org/download.php?sdk&version=1.8.0

However, I recommand to use the devlopment vesrion (CVS) if you can. The Rocket's module in 1.8.0 has a critical bug with the drag'n'drop feature, and we use it for inventory management.

_Careful Windows users: our code uses C++11 features only supported since MSVC2010: you need the unstable Panda3D 1.9.0 to compile on Windows. A prebuilt version is available at this link: http://buildbot.panda3d.org/downloads/dev_sdk_win_i386/builds/54/Panda3D-2013.10.06-54.exe_

**LibRocket 1.2.1**

http://librocket.com/download

**AngelScript == 2.25**

http://www.angelcode.com/angelscript/downloads.html

# Compiling on Windows #
In order to compile on Windows, you'll have to use Visual Studio 2010, which you can acquire with the Windows SDK 7.1.

Download this file:
http://rdb.name/thirdparty-vc10.7z

Copy the folder `win-libs-vc10/rocket/include/Rocket` to the repository in `3rd_parties/include`.

Copy the content of the folder `win-libs-vc10/rocket/lib` to the repository in `3rd_parties/lib`

Copy the folder `C:/Panda3D-1.9.0/include` to `3rd_parties/include` and rename it to `panda3d`.

Copy the content of the folder `C:/Panda3D-1.9.0/lib` to `3rd_parties/lib`.

Open the CMakeLists.txt at the root of the repository using CMake (http://www.cmake.org/files/v2.8/cmake-2.8.12-win32-x86.exe) and generate projects file for Visual Studio 2010.

Import the project in Visual Studio 2010, confiugre your build for Release (it is impossible to build in Debug for Windows), and you should be all set to go.

# Compiling on Linux #
There's a CMake file at the root of the git repository.

Make sure you have all the dependencies installed first (cmake, panda3d, librocket, angelscript).

Go into the build directory, and type 'cmake ..' to generate the Makefile.
Once it's over, type 'make' to compile the project.

# Executing #
You must execute the game from the "build" directory.