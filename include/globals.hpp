//Global includes
//Usually required only for "HAIL MICROSOFT!" situations

#ifndef  GLOBALS_HPP
# define GLOBALS_HPP

# define MODEL_TILE_PATH    "models/"
# define TEXTURE_PATH       "textures/"
# define TEXTURE_TILE_PATH  "textures/tiles/"

# define GET_ARRAY_SIZE(A) sizeof(A)/sizeof(A[0])

# define ABS(x) ((x) < 0 ? -(x) : (x))

# ifdef _WIN32
#  include <winsock2.h>
# endif

# include <panda3d/cmath.h>
# include <algorithm>
# include <string>
# define ForEach(stlContainer, functor) (std::for_each((stlContainer).begin(), (stlContainer).end(), (functor)))

/*
 * String Utils
 */
std::string humanize(const std::string&);
std::string underscore(const std::string&);

// Twilidoc cannot see Panda3D's PT macro.
# ifndef PT
#  define PT(PTTYPE) PointerTo< PTTYPE >
# endif

# if PANDA_MAJOR_VERSION > 1 || PANDA_MINOR_VERSION > 8
#  define INVENTORY_USE_DRAGDROP
# endif

#endif // GLOBALS_HPP
