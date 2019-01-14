/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: itkCubicSplineInterpolation3D.h,v $
  Language:  C++
  Date:      $Date: 2005-01-30 22:27:29 $
  Version:   $Revision: 1.3 $
  Author:    Pierre Fillard (fillard@cs.unc.edu)

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef ITKCUBICSPLINEINTERPOLATION3D_h
#define ITKCUBICSPLINEINTERPOLATION3D_h

#include <iostream>
#include <vector>

#include "vnl/vnl_math.h"
#include "itkTubeSpatialObjectPoint.h"
#include "itkTubeSpatialObject.h"
#include "itkContinuousIndex.h"
#include "itkCubicSplineInterpolation1D.h"
#include <stdio.h>
#include <sovWin32Header.h>

namespace itk
{

class SOVCommon_EXPORT CubicSplineInterpolation3D
{

public:

  CubicSplineInterpolation3D();
  ~CubicSplineInterpolation3D();
  
  typedef itk::Point<float, 3>                     MyPoint3D;
  typedef itk::TubeSpatialObjectPoint<3>           TubePoint;
  typedef std::vector<TubePoint>                   TubePointList;
  typedef itk::TubeSpatialObjectPoint<2>           TubePoint2D;
  typedef std::vector<TubePoint2D>                 TubePointList2D;
  typedef std::list<MyPoint3D>                     MyListPoint3D;
  typedef itk::Point<float, 1>                     MyPoint1D;
  typedef std::list<MyPoint1D>                     MyListPoint1D;
  typedef itk::Point<float, 2>                     MyPoint2D;
  typedef std::list<MyPoint2D>                     MyListPoint2D;
  typedef itk::TubeSpatialObject<3>                TubeType3D;

  typedef itk::ContinuousIndex<float, 1>           MyContinuousIndex;                   
  
  void SetInput(MyListPoint3D data);
  void SetInput(TubePointList data);
  void SetInput(TubeType3D* tube);
  void SetNbPointsInterpolated(float n);
  void SetRed(float r);
  void SetGreen(float g);
  void SetBlue(float b);

  MyListPoint1D Parametric(MyListPoint3D data);
  
  MyListPoint3D GetInterpolatedPointList();
  TubePointList GetInterpolatedTubePointList();

  double GetLength()
    { return m_Length; };


private:
  
  CubicSplineInterpolation1D m_1DSplineX;
  CubicSplineInterpolation1D m_1DSplineY;
  CubicSplineInterpolation1D m_1DSplineZ;

  float m_NPointsInterpolated;

  //for the tubes:
  float m_R; // RGB
  float m_B;
  float m_G;

  double m_Length;
    
};

} // end of namespace



#endif
