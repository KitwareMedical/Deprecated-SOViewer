PROJECT(ExternalRendering)

IF (NOT SOV_SOURCE_DIR)
  FIND_PACKAGE(SOV REQUIRED)
  INCLUDE(${SOV_USE_FILE})
ENDIF (NOT SOV_SOURCE_DIR)

INCLUDE(${CMAKE_ROOT}/Modules/FindITK.cmake)
IF(USE_ITK_FILE)
  INCLUDE(${USE_ITK_FILE})
ENDIF(USE_ITK_FILE)

INCLUDE_DIRECTORIES(
  ${ITK_SOURCE_DIR}/Code/SpatialObject
  ${ExternalRendering_BINARY_DIR}
  )

LINK_DIRECTORIES(
  ${itkUNC_BINARY_DIR}/Code/SpatialObject
  ${itkUNC_BINARY_DIR}/Auxiliary/MetaIO/src
  )

# the order of the library is IMPORTANT !
LINK_LIBRARIES(
  ${ITK_LIBRARIES}
  SOViewer
  ${GLU_LIBRARY}
  ${GLUT_LIBRARY}
  )

IF(USE_VTK)
  INCLUDE(${CMAKE_ROOT}/Modules/FindVTK.cmake)
  IF(USE_VTK_FILE)
    INCLUDE(${USE_VTK_FILE})
    LINK_LIBRARIES(
      ${VTK_LIBRARIES}
      )
    ADD_DEFINITIONS(-DUSE_VTK4)
  ENDIF(USE_VTK_FILE)
ENDIF(USE_VTK)

SET(ExternalRendering_SRCS
  main
  )

ADD_EXECUTABLE( ExternalRendering ${ExternalRendering_SRCS})
