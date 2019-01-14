/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: itkCubicSplineInterpolation1D.h,v $
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
#ifndef CUBICSPLINEINTERPOLATION1D_H
#define CUBICSPLINEINTERPOLATION1D_H

#include <iostream>
#include <list>
#include <vector>

#include "itkPoint.h"
#include "itkTubeSpatialObjectPoint.h"
#include "itkContinuousIndex.h"
#include <vnl/vnl_matrix.h> 
#include <vnl/algo/vnl_matrix_inverse.h> 
#include <vnl/vnl_math.h> 
#include <sovWin32Header.h>

namespace itk
{

class SOVCommon_EXPORT CubicSplineInterpolation1D
{
public:

    typedef itk::Point<float,2>                 MyPoint;
    typedef itk::TubeSpatialObjectPoint<2>      MyTubePoint;
    typedef std::list<MyPoint>                  MyListPoint;
    typedef std::vector<MyTubePoint>            MyListTubePoint;
    typedef itk::ContinuousIndex<float, 1>      MyIndex;
    typedef vnl_matrix<float>                   Matrix;
    typedef vnl_matrix_inverse<float>           MatrixInverse;
    
    
    CubicSplineInterpolation1D();
    ~CubicSplineInterpolation1D();
    CubicSplineInterpolation1D(MyListPoint data);

    void SetInput(MyListPoint data);
    void SetInput(MyListTubePoint data);
        
    unsigned int FindPositionIndex(MyIndex index, unsigned int i1, unsigned int i2);

    float GetValueAtContinuousPosition(MyIndex index);
    float GetRadiusAtIndex(MyIndex index);


private:

    float * m_DataX;
    float * m_DataY;
    int m_Dim;
    // 3 arrays of coefficient
    float * m_H;
    float * m_B;
    float * m_Fsec;
    float * m_Radius;
    // 1 Matrix
    Matrix  m_Mat;

protected:
  void DefineCoeff(void);


};

} // end of namespace



#endif
