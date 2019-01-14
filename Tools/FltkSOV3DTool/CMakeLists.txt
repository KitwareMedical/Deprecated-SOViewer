IF(USE_FLTK)
  IF(HAS_FLTK)

  INCLUDE_DIRECTORIES(
      ${FTLK_INCLUDE_DIR}
      ${SOV_SOURCE_DIR}/Viewer/Fltk/3D
      ${SOV_BINARY_DIR}/Viewer/Fltk/3D
    )

   IF(USE_VTK)
      IF(VTK_FOUND)

      ADD_EXECUTABLE(
        FltkSOV3DTool
        main.cxx
      )

      TARGET_LINK_LIBRARIES(FltkSOV3DTool FltkSOViewer3D)

      ENDIF(VTK_FOUND)
    ENDIF(USE_VTK)
  ENDIF(HAS_FLTK)
ENDIF(USE_FLTK)



