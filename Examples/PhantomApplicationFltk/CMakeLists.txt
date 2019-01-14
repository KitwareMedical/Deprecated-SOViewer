IF(USE_FLTK)

  INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)
  IF(HAS_FLTK)

PROJECT(PhantomApplicationFLTK)
INCLUDE(${SOV_SOURCE_DIR}/FindGHOST.cmake)

INCLUDE_DIRECTORIES(
  ${GHOST_INCLUDE_PATH}
)
 ADD_DEFINITIONS(-DUSE_PHANTOM)
 
    INCLUDE(${CMAKE_ROOT}/Modules/FindITK.cmake)
    IF(USE_ITK_FILE)
      INCLUDE(${USE_ITK_FILE})
    ENDIF(USE_ITK_FILE)
        
    INCLUDE_DIRECTORIES(
      ${ITK_SOURCE_DIR}/Code/SpatialObject
      ${FLTK_INCLUDE_PATH}
      ${PhantomApplicationFLTK_BINARY_DIR}
    )
  
    LINK_DIRECTORIES(
      ${itkUNC_BINARY_DIR}/Code/SpatialObject
    )
  
    # the order of the library is IMPORTANT !
    LINK_LIBRARIES(
      ${GHOST_LIBRARY} 
      ${ITK_LIBRARIES}
      SOViewer
      ${FLTK_LIBRARY}
    )
  
    IF(USE_VTK)
      INCLUDE(${CMAKE_ROOT}/Modules/FindVTK.cmake)
      IF(USE_VTK_FILE)
        INCLUDE(${USE_VTK_FILE})
        ADD_DEFINITIONS(-DUSE_VTK4)
      ENDIF(USE_VTK_FILE)
    ENDIF(USE_VTK)
  
    SET(PhantomApplicationFLTK_GUI_SRCS
      PhantomAppGUI.fl
    )
  
    SET(PhantomApplicationFLTK_SRCS
      main.cxx
      PhantomAppGUIControls.cxx
    )

  IF(CYGWIN)
    LINK_LIBRARIES(
      ole32 uuid comctl32 wsock32 supc++ -lm -lgdi32
      )
  ENDIF(CYGWIN)

  IF(WIN32)
   SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcirt.lib")
  ENDIF(WIN32)

     FLTK_WRAP_UI(PhantomApplicationFLTK  ${PhantomApplicationFLTK_GUI_SRCS})
  
    ADD_EXECUTABLE( PhantomApplicationFLTK ${PhantomApplicationFLTK_SRCS}
            ${PhantomApplicationFLTK_FLTK_UI_SRCS}
     )
  
  ENDIF(HAS_FLTK)
ENDIF(USE_FLTK)
