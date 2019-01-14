IF(QT_FOUND)

  PROJECT(PhantomApplicationQt)

  INCLUDE(${itkUNC_SOURCE_DIR}/FindGHOST.cmake)
  
  INCLUDE(${CMAKE_ROOT}/Modules/FindITK.cmake)
  IF(USE_ITK_FILE)
    INCLUDE(${USE_ITK_FILE})
  ENDIF(USE_ITK_FILE)
  
 
  INCLUDE_DIRECTORIES(
    ${PhantomApplicationQt_BINARY_DIR}
    ${GHOST_INCLUDE_PATH} 
  )
  
  # the order of the library is IMPORTANT !
  LINK_LIBRARIES(
    ${ITK_LIBRARIES}
    SOViewer
    ${GHOST_LIBRARY} 
  )
  
  
  IF(USE_VTK)
    INCLUDE(${CMAKE_ROOT}/Modules/FindVTK.cmake)
    IF(USE_VTK_FILE)
      INCLUDE(${USE_VTK_FILE})
    ENDIF(USE_VTK_FILE)
    IF(VTK_FOUND)
      LINK_LIBRARIES(
        ${VTK_LIBRARIES}
      )
      ADD_DEFINITIONS(-DUSE_VTK)
    ENDIF(VTK_FOUND)
  ENDIF(USE_VTK)

  INCLUDE_DIRECTORIES(
   ${QT_INCLUDE_PATH}
  )
  
  LINK_LIBRARIES(
   ${QT_QT_LIBRARY}
  )
  
  SET(PhantomApplicationQt_GUI_SRCS 
    PhantomAppGUI.ui
  )

  IF(QT_UIC_EXECUTABLE)
    SET(QT_WRAP_UI "YES")
    QT_WRAP_UI(PhantomApplicationQt  PhantomApplicationQt_HDRS
               PhantomApplicationQt_SRCS  ${PhantomApplicationQt_GUI_SRCS} )
  ENDIF(QT_UIC_EXECUTABLE)

  SET(
    PhantomApplicationQt_SRCS
    ${PhantomApplicationQt_SRCS}
    main.cxx
    PhantomAppGUIControls.cxx
  )

  ADD_DEFINITIONS(-DQT_DLL)
  ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

  ADD_EXECUTABLE( PhantomApplicationQt ${PhantomApplicationQt_SRCS})

ENDIF(QT_FOUND)
