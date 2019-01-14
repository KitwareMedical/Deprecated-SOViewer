
PROJECT(ShellEx)

IF (NOT SOV_SOURCE_DIR)
  FIND_PACKAGE(SOV REQUIRED)
  INCLUDE(${SOV_USE_FILE})
ENDIF (NOT SOV_SOURCE_DIR)

#---------------------------------------------------------------------------

FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR "Cannot build without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

FIND_PACKAGE(FLTK)
IF(FLTK_FOUND)
  INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
ELSE(FLTK_FOUND)
  MESSAGE(FATAL_ERROR "Cannot build without FLTK.  Please set FLTK_DIR.")
ENDIF(FLTK_FOUND)

FIND_PACKAGE(VTK)
IF(VTK_FOUND)
  INCLUDE (${VTK_USE_FILE})
ELSE(VTK_FOUND)
  MESSAGE(FATAL_ERROR "Cannot build without ITK.  Please set VTK_DIR.")
ENDIF(VTK_FOUND)

# Just so the generated GUI can find the Base class
INCLUDE_DIRECTORIES(${ShellEx_SOURCE_DIR})

#---------------------------------------------------------------------------

FLTK_WRAP_UI(shellex ShellExplorerGUI.fl)

ADD_EXECUTABLE(shellex shellex.cxx 
                       ShellExplorerApp.cxx 
                       ShellExplorerBase.cxx
                       ${shellex_FLTK_UI_SRCS})

TARGET_LINK_LIBRARIES(shellex 
                      SOViewer
                      ITKCommon ITKIO ITKSpatialObject
                      ${GLU_LIBRARY}
                      ${GLUT_LIBRARY}
                      ${FLTK_LIBRARY} )

#---------------------------------------------------------------------------
