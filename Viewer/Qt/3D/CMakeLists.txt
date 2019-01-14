PROJECT( QtSpatialObjectViewer3D )

IF(USE_QT)
  IF(QT_FOUND)
    INCLUDE_DIRECTORIES(
      ${QT_INCLUDE_PATH}
      ${QT_INCLUDE_DIR}
      ${QtSpatialObjectViewer3D_BINARY_DIR}
    )
  
    LINK_LIBRARIES(
     ${QT_QT_LIBRARY}
     ${ITK_LIBRARIES}
     vtkHybrid
     SOViewer
    )
    
    # 
    # VTK with Qt
    #
    IF(USE_VTK)
      IF(VTK_FOUND)
        SET(ObjectViewer_MOC_SRCS 
          Display/Qt/solQtVTKDisplay.h
          Display/Qt/solQtGlDisplay.h
        ) 
      
      SET(QtSpatialObjectViewer3D_GUI_SRCS 
      QtSpatialObjectViewer3DGUI.ui
      )

  IF(QT_UIC_EXECUTABLE)
    SET(QT_WRAP_UI "YES")
    QT_WRAP_UI(QtSpatialObjectViewer3D QtSpatialObjectViewer3D_HDRS
               QtSpatialObjectViewer3D_SRCS  ${QtSpatialObjectViewer3D_GUI_SRCS} )
  ENDIF(QT_UIC_EXECUTABLE)

  SET(
    QtSpatialObjectViewer3D_SRCS
    ${QtSpatialObjectViewer3D_SRCS}
    main.cxx
    QtSpatialObjectViewer3D.cxx
  )

  ADD_DEFINITIONS(-DQT_DLL)
  ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

        ADD_EXECUTABLE(
        QtSpatialObjectViewer3D 
        ${QtSpatialObjectViewer3D_SRCS}
        )

      ENDIF(VTK_FOUND)
    ENDIF(USE_VTK)
  ENDIF(QT_FOUND)
ENDIF(USE_QT)
