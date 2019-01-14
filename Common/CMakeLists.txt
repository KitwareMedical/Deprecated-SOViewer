SET(SOVCommon_SRCS
  sovViewerFactoryBase.cxx
  itkCubicSplineInterpolation1D.cxx
  itkCubicSplineInterpolation3D.cxx
  itkUltraSoundProbeSpatialObject.cxx
)


ADD_LIBRARY(SOVCommon ${SOVCommon_SRCS})
