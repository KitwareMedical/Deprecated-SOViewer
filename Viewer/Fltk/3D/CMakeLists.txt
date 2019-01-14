IF(USE_FLTK)
  IF(HAS_FLTK)
 
  PROJECT( FltkSOViewer3D )

  INCLUDE_DIRECTORIES(
      ${FTLK_INCLUDE_DIR}
      ${FltkSOViewer3D_SOURCE_DIR}
      ${FltkSOViewer3D_BINARY_DIR}
    )

    # 
    # VTK
    #
    IF(USE_VTK)
      IF(VTK_FOUND)

      SET(FltkSOViewer3D_GUI_SRCS 
      FltkSpatialObjectViewer3DGUI.fl
      )

      SET(
        FltkSOViewer3D_SRCS
        ${FltkSOViewer3D_SRCS}
        FltkSpatialObjectViewer3D.cxx
        ManagerInteractor.cxx
        ManagerInteractor.h
      )

     FLTK_WRAP_UI(FltkSOViewer3D ${FltkSOViewer3D_GUI_SRCS})
 
      ADD_LIBRARY(
        FltkSOViewer3D
        ${FltkSOViewer3D_SRCS}
        ${FltkSOViewer3D_FLTK_UI_SRCS}
      )
      ADD_DEPENDENCIES(FltkSOViewer3D SpatialObjectManager)
      TARGET_LINK_LIBRARIES(FltkSOViewer3D SOViewer)
      TARGET_LINK_LIBRARIES(FltkSOViewer3D SpatialObjectManager)

      ENDIF(VTK_FOUND)
    ENDIF(USE_VTK)
  ENDIF(HAS_FLTK)
ENDIF(USE_FLTK)



