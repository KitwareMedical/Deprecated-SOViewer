PROJECT(SpatialObjectManager)

INCLUDE(${CMAKE_ROOT}/Modules/FindITK.cmake)
IF(USE_ITK_FILE)
  INCLUDE(${USE_ITK_FILE})

  IF(USE_FLTK)

    INCLUDE(${CMAKE_ROOT}/Modules/FindFLTK.cmake)
    IF(HAS_FLTK)

      LINK_LIBRARIES(
        ${ITK_LIBRARIES}
        ${FLTK_LIBRARIES}  
      )

      INCLUDE_DIRECTORIES(
        ${FLTK_INCLUDE_PATH}
        ${itkUNC_SOURCE_DIR}/Auxiliary/SpatialObjectManager
        ${ITK_SOURCE_DIR}/Code/SpatialObject
      )

      SET(SOManager_GUI_SRCS
        SpatialObjectManagerGUI.fl
      )

     SET(SOManager_SRCS
        main.cxx
        SpatialObjectManager.h 
        SpatialObjectManager.cxx 
        Fl_ToggleTree.cxx
        Fl_Tree.cxx
      )

    FLTK_WRAP_UI(SpatialObjectManager ${SOManager_GUI_SRCS})   
 
      ADD_LIBRARY(
        SpatialObjectManager
        ${SOManager_SRCS}
        ${SpatialObjectManager_FLTK_UI_SRCS}
      )

     

    ENDIF(HAS_FLTK)
  ENDIF(USE_FLTK)
ENDIF(USE_ITK_FILE)
