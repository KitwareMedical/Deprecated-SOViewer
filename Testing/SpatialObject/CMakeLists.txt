INCLUDE(${CMAKE_ROOT}/Modules/FindITK.cmake)
IF(USE_ITK_FILE)
  INCLUDE(${USE_ITK_FILE})
ENDIF(USE_ITK_FILE)


INCLUDE_DIRECTORIES(
  ${ITK_SOURCE_DIR}/Code/SpatialObject
  ${ITK_SOURCE_DIR}/Code/Common
  ${ITK_SOURCE_DIR}/Code/Numerics/vxl/vnl
)

LINK_LIBRARIES(
  ${ITK_LIBRARIES}
)

SET(SPATIALOBJECTS_OBJECT_TESTS  ${CXX_TEST_PATH}/SOVSpatialObjectTests)


ADD_TEST( itkSpatialObjectMemoryTest ${SPATIALOBJECTS_OBJECT_TESTS}
          itkSpatialObjectMemoryTest)
ADD_TEST( itkSparsePointsSpatialObjectTest ${SPATIALOBJECTS_OBJECT_TESTS}
          itkSparsePointsSpatialObjectTest)

SET(
  SpatialObject_SRCS
  itkSpatialObjectMemoryTest.cxx
  itkSparsePointsSpatialObjectTest.cxx
)

ADD_EXECUTABLE( SOVSpatialObjectTests SOVSpatialObjectTests.cxx
                ${SpatialObject_SRCS})
