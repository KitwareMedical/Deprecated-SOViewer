PROJECT( FltkSOViewer2D )

IF(USE_FLTK)
  INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)
  IF(HAS_FLTK)
    IF(USE_VTK)
      IF(VTK_FOUND)
        INCLUDE_DIRECTORIES(
          ${FTLK_INCLUDE_DIR}
          ${FltkSOViewer2D_SOURCE_DIR}
          ${FltkSOViewer2D_BINARY_DIR}
          )
    
        SET(FltkSOViewer2D_GUI_SRCS 
          FltkSpatialObjectViewer2DGUI.fl
          )
        
        SET(
          FltkSOViewer2D_LIB_SRCS
          FltkSpatialObjectViewer2D.cxx
          FltkSpatialObjectViewer2D.h
          )

        FLTK_WRAP_UI(FltkSOViewer2D ${FltkSOViewer2D_GUI_SRCS})
        
        ADD_LIBRARY(FltkSOViewer2D ${FltkSOViewer2D_LIB_SRCS}  ${FltkSOViewer2D_FLTK_UI_SRCS})
        TARGET_LINK_LIBRARIES(FltkSOViewer2D SOViewer)
      ENDIF(VTK_FOUND)
    ENDIF(USE_VTK)
  ENDIF(HAS_FLTK)
ENDIF(USE_FLTK)

