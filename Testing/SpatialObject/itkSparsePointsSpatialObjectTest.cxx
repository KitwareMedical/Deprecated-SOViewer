#ifdef WIN32 
#pragma warning (disable : 4786)
#endif 

#include "itkSparsePointsSpatialObject.h"

int itkSparsePointsSpatialObjectTest(int, char *[])
  {
  typedef itk::SparsePointsSpatialObject<3>        SparsePointsType;
  typedef SparsePointsType::SparsePointListType    PointListType;
  typedef SparsePointsType::SparsePointType        PointType;

  SparsePointsType::Pointer objP = SparsePointsType::New();

  std::cout << "Test initialization:" << std::endl;
  std::cout << "   Init # points: " << objP->GetNumberOfPoints() << std::endl;

  std::cout << "Create a point" << std::endl;
  PointType * pointP = new SparsePointsType::SparsePointType;

  std::cout << "Adding point" << std::endl;
  objP->GetPoints().push_back(*pointP);

  std::cout << "  New # points: " << objP->GetNumberOfPoints() << std::endl;

  return 0;
  }
