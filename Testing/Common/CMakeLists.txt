INCLUDE_DIRECTORIES(
  ${ITK_SOURCE_DIR}/Code/Common
  ${ITK_SOURCE_DIR}/Code/Numerics/vxl/vnl
)

LINK_LIBRARIES(
${ITK_LIBRARIES}
)

SET(COMMON_OBJECT_TESTS  ${CXX_TEST_PATH}/SOVCommonTests)

ADD_TEST(itkMemoryLeakTest ${COMMON_OBJECT_TESTS} 
         itkMemoryLeakTest)

SET(
 Common_SRCS
 itkMemoryLeakTest.cxx
)

ADD_EXECUTABLE( SOVCommonTests SOVCommonTests.cxx ${Common_SRCS})
