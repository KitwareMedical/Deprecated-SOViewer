IF(USE_FLTK)

  INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)

  IF(HAS_FLTK)

  PROJECT(  FltkSOViewerSlicer )

  INCLUDE_DIRECTORIES(
    ${FTLK_INCLUDE_DIR}
    ${FltkSOViewerSlicer_BINARY_DIR}
    )

  # the order of the library is IMPORTANT !
    LINK_LIBRARIES(
      ${ITK_LIBRARIES}
      ${GLU_LIBRARY}
      ${GLUT_LIBRARY}
      ${FLTK_LIBRARY}
    )

IF(CYGWIN)
  LINK_LIBRARIES(
ole32 uuid comctl32 wsock32 supc++ -lm -lgdi32)
ENDIF(CYGWIN)


    # 
    # VTK
    #

    IF(USE_VTK)

      IF(VTK_FOUND) 

         
      SET( FltkSOViewerSlicer_GUI_SRCS 

      FltkSpatialObjectViewerSlicerGUI.fl

      )


  SET(

    FltkSOViewerSlicer_SRCS

    FltkSpatialObjectViewerSlicer.cxx
  )

    FLTK_WRAP_UI(FltkSOViewerSlicer ${FltkSOViewerSlicer_GUI_SRCS})
    ADD_LIBRARY(FltkSOViewerSlicer ${FltkSOViewerSlicer_SRCS}
               ${FltkSOViewerSlicer_FLTK_UI_SRCS}
               )


    TARGET_LINK_LIBRARIES(FltkSOViewerSlicer SOViewer)
    TARGET_LINK_LIBRARIES(FltkSOViewerSlicer SpatialObjectManager)

      ENDIF(VTK_FOUND)
    ENDIF(USE_VTK)
  ENDIF(HAS_FLTK)
ENDIF(USE_FLTK)



