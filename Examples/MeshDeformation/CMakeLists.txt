IF(USE_FLTK)

  INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)
  INCLUDE (${CMAKE_ROOT}/Modules/FindGLUT.cmake)

  IF(HAS_FLTK)
    PROJECT(MeshDeformationExample)
    
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
      ${FLTK_INCLUDE_DIR}
      ${MeshDeformationExample_BINARY_DIR}
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
      ${FLTK_LIBRARY}
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
    
    SET(MeshDeformationExample_GUI_SRCS
      MeshDeformationGUI.fl
      )
    
    SET(MeshDeformationExample_SRCS
      main
      MeshDeformationGUIControls
      )


    FLTK_WRAP_UI(MeshDeformationExample  ${MeshDeformationExample_GUI_SRCS})
    
    ADD_EXECUTABLE( MeshDeformationExample ${MeshDeformationExample_SRCS}
      ${MeshDeformationExample_FLTK_UI_SRCS} )
    
  ENDIF(HAS_FLTK)
ENDIF(USE_FLTK)
