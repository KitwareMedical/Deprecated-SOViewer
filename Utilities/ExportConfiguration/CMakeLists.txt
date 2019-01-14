# Generate the SOVConfig.cmake file in the build tree. Also configure
# one for installation. The file tells external projects how to use
# SOV.

# Help store a literal dollar in a string.  CMake 2.2 allows escaped
# dollars but we have to support CMake 2.0.
SET(DOLLAR "$")

#-----------------------------------------------------------------------------
# Settings shared between the build tree and install tree.

EXPORT_LIBRARY_DEPENDENCIES(
  ${SOV_BINARY_DIR}/SOVLibraryDepends.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(
  ${SOV_BINARY_DIR}/SOVBuildSettings.cmake)

#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The "use" file.
SET(SOV_USE_FILE_CONFIG 
  ${CMAKE_CURRENT_SOURCE_DIR}/UseSOV.cmake)

# The build settings file.
SET(SOV_BUILD_SETTINGS_FILE_CONFIG 
  ${SOV_BINARY_DIR}/SOVBuildSettings.cmake)

# The library directories.
SET(SOV_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# The runtime directories.
SET(SOV_RUNTIME_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# The include directories.
SET(SOV_INCLUDE_DIRS_CONFIG ${SOV_INCLUDE_PATH})

# The library dependencies file.
SET(SOV_LIBRARY_DEPENDS_FILE 
  ${SOV_BINARY_DIR}/SOVLibraryDepends.cmake)

# The ITK options.
SET(SOV_ITK_DIR_CONFIG ${ITK_DIR})

# The VTK options.
SET(SOV_VTK_DIR_CONFIG ${VTK_DIR})

# The FLTK options.
SET(SOV_FLTK_DIR_CONFIG ${FLTK_DIR})

# The build configuration information.
SET(SOV_CONFIGURATION_TYPES_CONFIG ${CMAKE_CONFIGURATION_TYPES})
SET(SOV_BUILD_TYPE_CONFIG ${CMAKE_BUILD_TYPE})

# Configure SOVConfig.cmake for the build tree.
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/SOVConfig.cmake.in
  ${SOV_BINARY_DIR}/SOVConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Settings specific to the install tree.
# Get back to Seb and Brad for this one, once you specify install targets
# in your CMakeLists.txt file
