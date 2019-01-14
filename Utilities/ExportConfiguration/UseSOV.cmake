#
# This module is provided as SOV_USE_FILE by SOVConfig.cmake.  
# It can be INCLUDEd in a project to load the needed compiler and linker
# settings to use SOV:
#   FIND_PACKAGE(SOV REQUIRED)
#   INCLUDE(${SOV_USE_FILE})
#

IF(NOT SOV_USE_FILE_INCLUDED)
  SET(SOV_USE_FILE_INCLUDED 1)

  # Load the compiler settings used for SOV.
  IF(SOV_BUILD_SETTINGS_FILE)
    INCLUDE(${CMAKE_ROOT}/Modules/CMakeImportBuildSettings.cmake)
    CMAKE_IMPORT_BUILD_SETTINGS(${SOV_BUILD_SETTINGS_FILE})
  ENDIF(SOV_BUILD_SETTINGS_FILE)

  # Add compiler flags needed to use SOV.
  SET(CMAKE_C_FLAGS 
    "${CMAKE_C_FLAGS} ${SOV_REQUIRED_C_FLAGS}")
  SET(CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} ${SOV_REQUIRED_CXX_FLAGS}")
  SET(CMAKE_EXE_LINKER_FLAGS 
    "${CMAKE_EXE_LINKER_FLAGS} ${SOV_REQUIRED_EXE_LINKER_FLAGS}")
  SET(CMAKE_SHARED_LINKER_FLAGS 
    "${CMAKE_SHARED_LINKER_FLAGS} ${SOV_REQUIRED_SHARED_LINKER_FLAGS}")
  SET(CMAKE_MODULE_LINKER_FLAGS 
    "${CMAKE_MODULE_LINKER_FLAGS} ${SOV_REQUIRED_MODULE_LINKER_FLAGS}")

  # Add include directories needed to use SOV.
  INCLUDE_DIRECTORIES(${SOV_INCLUDE_DIRS})

  # Add link directories needed to use SOV.
  LINK_DIRECTORIES(${SOV_LIBRARY_DIRS})

  # Use ITK.
  IF(NOT ITK_DIR)
    SET(ITK_DIR ${SOV_ITK_DIR})
    FIND_PACKAGE(ITK)
    IF(ITK_FOUND)
      INCLUDE(${ITK_USE_FILE})
    ELSE(ITK_FOUND)
      MESSAGE("ITK not found in SOV_ITK_DIR=\"${SOV_ITK_DIR}\".")
    ENDIF(ITK_FOUND)
  ENDIF(NOT ITK_DIR)

  # Use VTK.
  IF(SOV_USE_VTK)
    IF(NOT VTK_DIR)
      SET(VTK_DIR ${SOV_VTK_DIR})
      FIND_PACKAGE(VTK)
      IF(VTK_FOUND)
        INCLUDE(${VTK_USE_FILE})
      ELSE(VTK_FOUND)
        MESSAGE("VTK not found in SOV_VTK_DIR=\"${SOV_VTK_DIR}\".")
      ENDIF(VTK_FOUND)
    ENDIF(NOT VTK_DIR)
  ENDIF(SOV_USE_VTK)

  # Use FLTK.
  IF(SOV_USE_FLTK AND NOT SOV_NO_USE_FLTK)
    IF(NOT FLTK_DIR)
      SET(FLTK_DIR ${SOV_FLTK_DIR})
      FIND_PACKAGE(FLTK)
      IF(FLTK_FOUND)
        IF(FLTK11_USE_FILE)
          SET(FLTK11_FOUND ${FLTK_FOUND})
          INCLUDE(${FLTK11_USE_FILE})
        ELSE(FLTK11_USE_FILE)
          MESSAGE("FLTK11_USE_FILE not found.")
        ENDIF(FLTK11_USE_FILE)
      ELSE(FLTK_FOUND)
        MESSAGE("FLTK not found in SOV_FLTK_DIR=\"${SOV_FLTK_DIR}\".")
      ENDIF(FLTK_FOUND)
    ENDIF(NOT FLTK_DIR)
  ENDIF(SOV_USE_FLTK AND NOT SOV_NO_USE_FLTK)

  # Same thing should probably be done for Qt and Phantom, but
  # I do not know enough about those packages at the moment

ENDIF(NOT SOV_USE_FILE_INCLUDED)
