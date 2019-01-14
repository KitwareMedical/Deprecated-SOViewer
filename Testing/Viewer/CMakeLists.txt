IF(USE_FLTK)
  INCLUDE(${CMAKE_ROOT}/Modules/FindFLTK.cmake)
  IF(HAS_FLTK)
    INCLUDE_DIRECTORIES(
      ${FLTK_INCLUDE_DIR}
      ${SOV_SOURCE_DIR}/Viewer/Fltk/3D
      ${SOV_BINARY_DIR}/Viewer/Fltk/3D
      ${SOV_SOURCE_DIR}/Viewer/Fltk/2D
      ${SOV_BINARY_DIR}/Viewer/Fltk/2D
      )
    
    IF(USE_VTK)
      INCLUDE(${CMAKE_ROOT}/Modules/FindVTK.cmake)
      IF(USE_VTK_FILE)
        INCLUDE(${USE_VTK_FILE})
        ADD_DEFINITIONS(-DUSE_VTK) 
        
        SET(OBJECTVIEWERS_FLGL_TESTS 
          ${CXX_TEST_PATH}/sovFlGlRenderWindowTest)
        ADD_TEST( sovFlGlRenderWindowTest ${OBJECTVIEWERS_FLGL_TESTS}
          sovFlGlRenderWindowTest)
        ADD_EXECUTABLE( sovFlGlRenderWindowTest sovFlGlRenderWindowTest.cxx) 
        TARGET_LINK_LIBRARIES(sovFlGlRenderWindowTest FltkSOViewer2D)
        
        # BCC doesn't have enough memory to link this test
        IF(NOT BORLAND)
          SET(OBJECTVIEWERS_FLVTK_TESTS 
            ${CXX_TEST_PATH}/sovFlVTKRenderWindowTest)
          ADD_TEST(sovFlVTKRenderWindowTest ${OBJECTVIEWERS_FLVTK_TESTS}
            sovFlVTKRenderWindowTest)
          ADD_EXECUTABLE( sovFlVTKRenderWindowTest sovFlVTKRenderWindowTest.cxx) 
          TARGET_LINK_LIBRARIES(sovFlVTKRenderWindowTest FltkSOViewer3D)
        ENDIF(NOT BORLAND)
      ENDIF(USE_VTK_FILE)
    ENDIF(USE_VTK)
  ENDIF(HAS_FLTK)
ENDIF(USE_FLTK)
