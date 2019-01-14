/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSplineTubeVTKRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Pierre Fillard (fillard@cs.unc.edu)

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __SplineTubeVTKRenderMethod3D_h
#define __SplineTubeVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include "itkTubeSpatialObject.h"
#include "itkCubicSplineInterpolation1D.h"
#include "itkCubicSplineInterpolation3D.h"
#include "itkPoint.h"
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT SplineTubeVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef SplineTubeVTKRenderMethod3D         Self;
  typedef SmartPointer< Self >                 Pointer;
  typedef VTKRenderMethod3D                    Superclass;
  
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;

  typedef itk::CubicSplineInterpolation3D     MySpline3D;
  typedef itk::Point<float,3>                 MyPoint;
  typedef std::list<MyPoint>                  MyListPoint;

  itkNewMacro( SplineTubeVTKRenderMethod3D );

  itkTypeMacro( SplineTubeVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );
  virtual void Update();

  void SetNumberOfPoints(double n);

protected:

  SplineTubeVTKRenderMethod3D();

  ~SplineTubeVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;


  
  unsigned int m_NumberOfSides;
  double m_PointCoeff;

};

} // end of namespace sov

#endif //__SplineTubeVTKRenderMethod3D_h
