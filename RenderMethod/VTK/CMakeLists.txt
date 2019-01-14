IF(USE_VTK)
  INCLUDE(${CMAKE_ROOT}/Modules/FindVTK.cmake)
  IF(USE_VTK_FILE)
    INCLUDE(${USE_VTK_FILE})
    LINK_LIBRARIES(
      vtkRendering
      vtkGraphics
      vtkImaging
      vtkIO
      vtkCommon
      vtkFiltering
      vtkHybrid
    )
    ADD_DEFINITIONS(-DUSE_VTK)
    AUX_SOURCE_DIRECTORY(
      3D
      SOVVTKRenderMethod_SRCS
    )
    AUX_SOURCE_DIRECTORY(
      Slicer
      SOVVTKRenderMethod_SRCS
    )

 ENDIF(USE_VTK_FILE)
ENDIF(USE_VTK)

INCLUDE_DIRECTORIES(
  ${ITK_SOURCE_DIR}/Code/SpatialObject
)

ADD_LIBRARY(
  SOVVTKRenderMethod ${SOVVTKRenderMethod_SRCS}
)

TARGET_LINK_LIBRARIES(SOVVTKRenderMethod SOVCommon ITKSpatialObject)
