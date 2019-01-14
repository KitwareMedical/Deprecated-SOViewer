PROJECT(SampleApplicationQt)

IF (NOT SOV_SOURCE_DIR)
  FIND_PACKAGE(SOV REQUIRED)
  INCLUDE(${SOV_USE_FILE})
ENDIF (NOT SOV_SOURCE_DIR)

INCLUDE(${CMAKE_ROOT}/Modules/FindITK.cmake)
IF(USE_ITK_FILE)
  INCLUDE(${USE_ITK_FILE})
ENDIF(USE_ITK_FILE)

INCLUDE_DIRECTORIES(
  ${SampleApplicationQt_BINARY_DIR}
  )


LINK_DIRECTORIES(
  )


# the order of the library is IMPORTANT !
LINK_LIBRARIES(
  ${ITK_LIBRARIES}
  ${QT_LIBRARIES}
  SOViewer
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
ELSE(USE_VTK)
  INCLUDE(${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
ENDIF(USE_VTK)

IF(USE_QT)
  INCLUDE(${CMAKE_ROOT}/Modules/FindQt.cmake)
  INCLUDE_DIRECTORIES(
    ${QT_INCLUDE_PATH}
    ${QT_INCLUDE_DIR}
    )

  LINK_LIBRARIES(
    ${QT_QT_LIBRARY}
    )

  SET(SampleApplicationQt_GUI_SRCS 
    SampleAppGUI.ui
    )

  IF(QT_UIC_EXECUTABLE)
    SET(QT_WRAP_UI "YES")
    QT_WRAP_UI(SampleApplicationQt  SampleApplicationQt_HDRS
      SampleApplicationQt_SRCS  ${SampleApplicationQt_GUI_SRCS} )
  ENDIF(QT_UIC_EXECUTABLE)

  SET(
    SampleApplicationQt_SRCS
    ${SampleApplicationQt_SRCS}
    ${SampleApplicationQt_QT_UI_SRCS}
    main.cxx
    SampleAppGUIControls.cxx
    )

  ADD_DEFINITIONS(-DQT_DLL)
  ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

  ADD_EXECUTABLE( SampleApplicationQt ${SampleApplicationQt_SRCS})

ENDIF(USE_QT)
