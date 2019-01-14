#
# try to find Ghost
#
# GHOST_INCLUDE_PATH = where the Phantom include directory can be found
# GHOST_LIB_PATH     = where the GHOST library can be found
# GHOST_LIBRARY      = the name of the GHOST library to link with
#

FIND_PATH(GHOST_INCLUDE_PATH ghostGLDraw.h
"C:/Program Files/SensAble/GHOST/v4.0/include" 
)

FIND_PATH(GHOST_LIB_PATH ghostGLDraw.lib
"C:/Program Files/SensAble/GHOST/v4.0/lib" 
)

FIND_LIBRARY(GHOST_BASE_LIBRARY  NAMES GHOST40
     PATHS ${GHOST_LIB_PATH}
)

FIND_LIBRARY(GHOST_GL_DRAW_LIBRARY NAMES ghostGLDraw
     PATHS ${GHOST_LIB_PATH}
)

FIND_LIBRARY(GHOST_GL_MANAGER_LIBRARY NAMES ghostGLManager
     PATHS ${GHOST_LIB_PATH}
)

FIND_LIBRARY(GHOST_GLUT_MANAGER_LIBRARY NAMES ghostGLUTManager
     PATHS ${GHOST_LIB_PATH}
)

SET( GHOST_LIBRARY ${GHOST_BASE_LIBRARY} ${GHOST_GL_DRAW_LIBRARY} ${GHOST_GL_MANAGER_LIBRARY} ${GHOST_GLUT_MANAGER_LIBRARY} )

OPTION(USE_GHOST_WRAP "Use Ghost Wrap" 1)
IF(USE_GHOST_WRAP)
 
 FIND_PATH(GHOSTWRAP_INCLUDE_PATH GhostWrap.h
   "C:/Program Files/GhostWrap/include" 
 )

 FIND_PATH(GHOSTWRAP_HIDESTL_INCLUDE_PATH gwp_cleanup.h
   "C:/Program Files/GhostWrap/hide_stl_include" 
 )
 
 FIND_LIBRARY(GHOSTWRAP_LIBRARY NAMES GhostWrap
     PATHS  "C:/Program Files/GhostWrap/lib" 
 )

 SET(GHOST_INCLUDE_PATH ${GHOSTWRAP_INCLUDE_PATH} ${GHOSTWRAP_HIDESTL_INCLUDE_PATH} ${GHOST_INCLUDE_PATH} )
 SET( GHOST_LIBRARY ${GHOSTWRAP_LIBRARY} ${GHOST_LIBRARY} )

ENDIF(USE_GHOST_WRAP)





