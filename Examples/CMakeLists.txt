INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)
IF(USE_ITK_FILE)
  INCLUDE (${USE_ITK_FILE})
ENDIF(USE_ITK_FILE)

IF(USE_QT)
  IF(USE_VTK)
    SUBDIRS(
      SampleApplicationQt
    )
    IF(USE_PHANTOM)
      SUBDIRS(
        PhantomApplicationQt
      )
    ENDIF(USE_PHANTOM)
  ENDIF(USE_VTK)
ENDIF(USE_QT)

IF(FLTK_FOUND)
  IF(USE_VTK)
    SUBDIRS(
      SampleApplicationFltk
      shellex
      MeshDeformation
      ExternalRendering
    )
 IF(USE_PHANTOM)
      SUBDIRS(
        PhantomApplicationFLTK
      )
    ENDIF(USE_PHANTOM)
  ENDIF(USE_VTK)
ENDIF(FLTK_FOUND)

