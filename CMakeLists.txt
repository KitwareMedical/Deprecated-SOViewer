CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

PROJECT(SOV)

#-----------------------------------------------------------------------------
# To use this library in a larger project you will need to SUBDIR into this
# directory so that it gets built and then you will use the following variables
# in your CMakeLists files to get the proper include paths and libraries

SET(SOV_INCLUDE_PATH 
  "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}"
  CACHE INTERNAL "include paths for SOViewer"
 )

SET(SOV_LIBRARIES SOViewer CACHE INTERNAL "libraries for SOViewer")

#-----------------------------------------------------------------------------
# Version. An even minor number corresponds to releases.

SET(SOV_VERSION_MAJOR "1")
SET(SOV_VERSION_MINOR "0")
SET(SOV_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.

SET(SOL_VERSION_STRING "${SOL_VERSION_MAJOR}.${SOL_VERSION_MINOR}")


# Disable MSVC 8 warnings
IF(WIN32)
  OPTION(DISABLE_MSVC8_DEPRECATED_WARNINGS "Disable Visual Studio 8 deprecated warnings" ON)
  MARK_AS_ADVANCED(FORCE DISABLE_MSVC8_DEPRECATED_WARNINGS)  
  IF(DISABLE_MSVC8_DEPRECATED_WARNINGS)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
  ENDIF(DISABLE_MSVC8_DEPRECATED_WARNINGS)
ENDIF(WIN32)


#-----------------------------------------------------------------------------
# Output directories

IF(NOT LIBRARY_OUTPUT_PATH)
  SET(LIBRARY_OUTPUT_PATH ${SOV_BINARY_DIR}/bin CACHE INTERNAL 
    "Single output directory for building all libraries.")
ENDIF(NOT LIBRARY_OUTPUT_PATH)

IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET(EXECUTABLE_OUTPUT_PATH ${SOV_BINARY_DIR}/bin CACHE INTERNAL 
    "Single output directory for building all executables.")
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)

SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

#-----------------------------------------------------------------------------
# Shared or static

OPTION(BUILD_SHARED_LIBS "Build SOViewer with shared libraries." OFF)
SET(SOV_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

#-----------------------------------------------------------------------------
# Configure Dart testing support.

INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)
IF(BUILD_TESTING)
  ENABLE_TESTING()
  IF(NOT DART_ROOT)
    SET(DROP_LOCATION /DartClient/SOViewer)
  ENDIF(NOT DART_ROOT)
ENDIF(BUILD_TESTING)

#-----------------------------------------------------------------------------
# Test for some required system information.

INCLUDE(${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityC.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityCXX.cmake)

#-----------------------------------------------------------------------------
# Find ITK

FIND_PACKAGE(ITK REQUIRED)
IF(USE_ITK_FILE)
  INCLUDE(${USE_ITK_FILE})
  INCLUDE_DIRECTORIES(
    ${ITK_SOURCE_DIR}/Code/Common
    ${ITK_SOURCE_DIR}/Code/SpatialObject
    ${ITK_SOURCE_DIR}/Code/IO
    )
  LINK_LIBRARIES(${ITK_LIBRARIES})
ENDIF(USE_ITK_FILE)

#-----------------------------------------------------------------------------
# Use Phantom 

OPTION(USE_PHANTOM "Use SenSable Phantom" "OFF")
IF(USE_PHANTOM)
  INCLUDE(${SOV_SOURCE_DIR}/FindGHOST.cmake)
  SET(SOV_SRCS 
    Renderer/sovPhantomRenderer.cxx
    Renderer/sovPhantomRenderer.h
    )
  SET(SOV_INCLUDE_PATH ${SOV_INCLUDE_PATH}
    ${SOV_SOURCE_DIR}/RenderMethod/Phantom
    )
  INCLUDE_DIRECTORIES(
    ${GHOSTWRAP_INCLUDE_PATH}
    ${GHOST_INCLUDE_PATH}
    )
  ADD_DEFINITIONS(-DUSE_PHANTOM)
ENDIF(USE_PHANTOM)

#-----------------------------------------------------------------------------
# Include directories

SET(SOV_INCLUDE_PATH ${SOV_INCLUDE_PATH}
  ${SOV_BINARY_DIR}/Utilities/SpatialObjectManager
  ${SOV_SOURCE_DIR}/Common
  ${SOV_SOURCE_DIR}/Display
  ${SOV_SOURCE_DIR}/Display/Fltk
  ${SOV_SOURCE_DIR}/Display/Qt
  ${SOV_SOURCE_DIR}/Interactor
  ${SOV_SOURCE_DIR}/RenderMethod
  ${SOV_SOURCE_DIR}/RenderMethod/OpenGL
  ${SOV_SOURCE_DIR}/RenderMethod/OpenGL/2D
  ${SOV_SOURCE_DIR}/RenderMethod/OpenGL/Slicer
  ${SOV_SOURCE_DIR}/RenderMethod/Phantom
  ${SOV_SOURCE_DIR}/RenderMethod/VTK
  ${SOV_SOURCE_DIR}/RenderMethod/VTK/3D
  ${SOV_SOURCE_DIR}/RenderMethod/VTK/Slicer
  ${SOV_SOURCE_DIR}/Renderer
  ${SOV_SOURCE_DIR}/Utilities/FlTrackBall
  ${SOV_SOURCE_DIR}/Utilities/SpatialObjectManager
  ${SOV_SOURCE_DIR}/Window/Fltk
  ${SOV_SOURCE_DIR}/Window/Qt
  )

#-----------------------------------------------------------------------------
# Link directories

LINK_DIRECTORIES(
  ${SOV_BINARY_DIR}/bin
  ${SOV_BINARY_DIR}/RenderMethod/OpenGL
  ${SOV_BINARY_DIR}/RenderMethod/VTK
  )

IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      SET(CMAKE_EXE_LINKER_FLAGS 
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcirt.lib")
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)

#-----------------------------------------------------------------------------
# VTK Support ?

OPTION(USE_VTK "Use VTK library" "OFF")
IF(USE_VTK)
  FIND_PACKAGE(VTK REQUIRED)
  INCLUDE(${USE_VTK_FILE})
  IF(VTK_FOUND)
    LINK_LIBRARIES(${VTK_LIBRARIES})
    ADD_DEFINITIONS(-DUSE_VTK)
    SET(SOV_INCLUDE_PATH ${SOV_INCLUDE_PATH}
      ${SOV_SOURCE_DIR}/RenderMethod/VTK/Slicer
      ${SOV_SOURCE_DIR}/RenderMethod/VTK/3D
      )
    SET(SOV_SRCS ${SOV_SRCS}
      Renderer/sovVTKRenderer3D.cxx
      Renderer/sovVTKRendererSlicer.cxx
      Renderer/sovColorScheme.cxx
      Renderer/sovColorSchemeXML.cxx
      Interactor/sovVTKMouseInteractor.cxx
      Interactor/sovVTKMouseInteractor.h
      Interactor/sovGlSlicerKeyboardInteractor.h
      Interactor/sovGlSlicerKeyboardInteractor.cxx
      Interactor/sovGlSlicerMouseInteractor.h
      Interactor/sovGlSlicerMouseInteractor.cxx
      Interactor/sovVTKMouseImageInteractor.cxx
      Interactor/sovVTKMouseImageInteractor.h
      )
  ENDIF(VTK_FOUND)
ENDIF(USE_VTK)

#-----------------------------------------------------------------------------
# We need OpenGL

# Problem is, we *might* also need VTK, and by linking against VTK 
# we will automatically link against the OpenGL libs that were
# used in VTK, the X11 ones for example. On the other hand, other
# parts of SOV do not use VTK but still require OpenGL, so we need
# to find it, but we should go the extra mile and make sure we
# find the one that is similar to the one used in VTK. This is usually
# only a problem for MacOSX, so copy the CMakeLists.txt code that
# is already in VTK.

IF(USE_VTK AND VTK_FOUND)
  # At the moment CMake's FindOpenGL considers OpenGL should be found
  # in the framework version on OSX. This is a reasonable assumption for
  # few people are going to use X. The module warns that if X is to be
  # used, one has to set the libs and include dir manually, which is
  # exactly what we are going to do below.
  IF(APPLE AND VTK_USE_X)
    FIND_PATH(OPENGL_INCLUDE_DIR GL/gl.h 
      /usr/X11R6/include)
    FIND_PATH(OPENGL_xmesa_INCLUDE_DIR GL/xmesa.h 
      /usr/X11R6/include)
    FIND_LIBRARY(OPENGL_gl_LIBRARY NAMES GL MesaGL 
      PATHS /usr/lib /usr/local/lib /usr/X11R6/lib)
    FIND_LIBRARY(OPENGL_glu_LIBRARY NAMES GLU MesaGLU 
      PATHS ${OPENGL_gl_LIBRARY} /usr/lib /usr/local/lib /usr/X11R6/lib)
  ENDIF(APPLE AND VTK_USE_X)
ENDIF(USE_VTK AND VTK_FOUND)

FIND_PACKAGE(OpenGL)
FIND_PACKAGE(GLU)

INCLUDE_DIRECTORIES(${GLU_LIBRARY_PATH})
IF(OPENGL_INCLUDE_DIR)
  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
ENDIF(OPENGL_INCLUDE_DIR)
LINK_LIBRARIES(${OPENGL_LIBRARY} ${GLU_LIBRARY})
  
#-----------------------------------------------------------------------------
# FLTK Support ?

OPTION(USE_FLTK "Use FLTK library" "OFF")
IF(USE_FLTK)
  FIND_PACKAGE(FLTK REQUIRED)
  IF(HAS_FLTK)
    INCLUDE_DIRECTORIES(
      ${FLTK_INCLUDE_PATH}
      ${FLTK_INCLUDE_DIR}
      )
    SET(SOV_SRCS ${SOV_SRCS}
      Display/Fltk/sovFlGlDisplay.cxx
      )
    LINK_DIRECTORIES(${itkUNC_BINARY_DIR}/FlTrackBall)
    LINK_LIBRARIES(
      ${FLTK_LIBRARIES}
      ${OPENGL_LIBRARIES}
      )
    IF(CYGWIN)
      LINK_LIBRARIES(ole32 uuid comctl32 wsock32 supc++ -lm -lgdi32)
    ENDIF(CYGWIN)

    # VTK with FLTK
    IF(USE_VTK)
      IF(VTK_FOUND)
        SET(SOV_SRCS ${SOV_SRCS}
          Display/Fltk/sovFlVTKDisplay.cxx
          )
      ENDIF(VTK_FOUND)
    ENDIF(USE_VTK)
  ENDIF(HAS_FLTK)
ENDIF(USE_FLTK)

#-----------------------------------------------------------------------------
# Qt Support ?

OPTION(USE_QT "Use Qt library" "OFF")
IF(USE_QT)
  FIND_PACKAGE(Qt REQUIRED)
  IF(QT_FOUND)
    INCLUDE_DIRECTORIES(
      ${QT_INCLUDE_PATH}
      ${QT_INCLUDE_DIR}
      )
    LINK_LIBRARIES(${QT_QT_LIBRARY})
    
    # OpenGL with Qt
    SET(SOV_SRCS ${SOV_SRCS} 
      Display/Qt/sovQtGlDisplay
      )
    
    # VTK with Qt

    IF(USE_VTK)
      IF(VTK_FOUND)
        SET(SOV_MOC_SRCS 
          Display/Qt/sovQtVTKDisplay.h
          Display/Qt/sovQtGlDisplay.h
          ) 
        IF(QT_UIC_EXECUTABLE)
          QT_WRAP_CPP(SOViewer SOViewermoced_SRCS ${SOV_MOC_SRCS})
        ENDIF(QT_UIC_EXECUTABLE)
        ADD_DEFINITIONS(-DQT_DLL)
        ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)
        SET(SOV_SRCS ${SOV_SRCS}
          ${SOViewermoced_SRCS} 
          Display/Qt/sovQtVTKDisplay.cxx
          )
      ENDIF(VTK_FOUND)
    ENDIF(USE_VTK)
  ENDIF(QT_FOUND)
ENDIF(USE_QT)


#-----------------------------------------------------------------------------
# General OpenGL Support ?

SET(BUILD_GL 0)
IF(USE_VTK AND VTK_FOUND)
  SET(BUILD_GL 1)
ELSE(USE_VTK AND VTK_FOUND)
  # If we are not using VTK, we need OpenGL explicitly
  IF(NOT WIN32 AND NOT OPENGL_INCLUDE_DIR)
    SET(BUILD_GL 0)
  ENDIF(NOT WIN32 AND NOT OPENGL_INCLUDE_DIR)
ENDIF(USE_VTK AND VTK_FOUND)

IF(BUILD_GL)
  SET(SOV_SRCS ${SOV_SRCS}
    Renderer/sovGlRendererSlicer.cxx
    Renderer/sovGlRenderer2D.cxx
    )
ENDIF(BUILD_GL)
    
SET(SOV_SRCS ${SOV_SRCS}
  Renderer/sovNDimensionalRenderer.cxx
  )

#-----------------------------------------------------------------------------
# Build sub-libraries

INCLUDE_DIRECTORIES(${SOV_INCLUDE_PATH})

SUBDIRS(
  Common 
  RenderMethod
  Utilities
  Viewer
  Tools
  )

ADD_LIBRARY(SOViewer ${SOV_SRCS})
TARGET_LINK_LIBRARIES(SOViewer SOVCommon)

IF(VTK_FOUND)
  TARGET_LINK_LIBRARIES(SOViewer SOVVTKRenderMethod)
  TARGET_LINK_LIBRARIES(SOViewer SOVOpenGLRenderMethod)
ENDIF(VTK_FOUND)

IF(USE_PHANTOM)
  TARGET_LINK_LIBRARIES(SOViewer SOVPhantomRenderMethod)
ENDIF(USE_PHANTOM)

IF(USE_FLTK)
  IF(HAS_FLTK)
   TARGET_LINK_LIBRARIES(SOViewer FlTrackBall)
   TARGET_LINK_LIBRARIES(SOViewer SpatialObjectManager)
  ENDIF(HAS_FLTK)  
ENDIF(USE_FLTK)

#-----------------------------------------------------------------------------
# Build testing and examples

IF(BUILD_TESTING)
  SUBDIRS(Testing)
ENDIF(BUILD_TESTING)

OPTION(BUILD_EXAMPLES "Build the examples" "OFF")
IF(BUILD_EXAMPLES)
  SUBDIRS(Examples)
ENDIF(BUILD_EXAMPLES)

#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.

CONFIGURE_FILE(
  ${SOV_SOURCE_DIR}/sovConfigure.h.in 
  ${SOV_BINARY_DIR}/sovConfigure.h)

CONFIGURE_FILE(
  ${SOV_SOURCE_DIR}/Utilities/Doxygen/doxygen.config.in
  ${SOV_BINARY_DIR}/doxygen.config)

CONFIGURE_FILE(
  ${SOV_SOURCE_DIR}/Utilities/Doxygen/itkdoxygen.pl.in
  ${SOV_BINARY_DIR}/itkdoxygen.pl)

#-----------------------------------------------------------------------------
# Build the Software Guide?

OPTION(BUILD_SOFTWAREGUIDE "Build the Software Guide" "OFF")
IF(BUILD_SOFTWAREGUIDE)
  SUBDIRS(SoftwareGuide)
ENDIF(BUILD_SOFTWAREGUIDE)

# --------------------------------------------------------------------------
# Configure the export configuration

SUBDIRS(Utilities/ExportConfiguration)
