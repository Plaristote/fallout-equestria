//Global includes
//Usually required only for "HAIL MICROSOFT!" situations

#ifndef  GLOBALS_HPP
# define GLOBALS_HPP

# define MODEL_TILE_PATH    "models/"
# define TEXTURE_PATH       "textures/"
# define TEXTURE_TILE_PATH  "textures/tiles/"

# define GET_ARRAY_SIZE(A) sizeof(A)/sizeof(A[0])

# define ABS(x) ((x) < 0 ? -(x) : (x))

# include <panda3d/cmath.h>
# include <algorithm>
# define ForEach(stlContainer, functor) (std::for_each((stlContainer).begin(), (stlContainer).end(), (functor)))

# ifndef TRUE_SQRT
#  define SQRT my_sqrt
float my_sqrt(const float x);
# else
#  define SQRT sqrt
# endif

#endif // GLOBALS_HPP
