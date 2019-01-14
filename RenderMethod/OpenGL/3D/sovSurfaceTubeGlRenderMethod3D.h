/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceTubeGlRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __SurfaceTubeGlRenderMethod3D_h
#define __SurfaceTubeGlRenderMethod3D_h

#include "sovGlRenderMethod3D.h"
#include "itkTubeSpatialObject.h"

#include <FL/gl.h>
#include <gle.h>

namespace sov
{

class SurfaceTubeGlRenderMethod3D
:public GlRenderMethod3D
{

public: 

  typedef SurfaceTubeGlRenderMethod3D         Self;
  typedef SmartPointer< Self >                Pointer;
  typedef GlRenderMethod3D                    Superclass;
  
  typedef itk::TubeSpatialObject<3,3>         TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::ChildrenListType          TubeListType;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType::Pointer              TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;
  typedef std::list< double[500][3] >         GlePointListType;
  typedef std::list< double[500] >            GleRadiusListType;
  typedef std::list< unsigned int >           GleSizeListType;

  itkNewMacro( SurfaceTubeGlRenderMethod3D );

  itkTypeMacro( SurfaceTubeGlRenderMethod3D, GlRenderMethod3D );

  virtual void draw( void );

protected:
  
  double m_Point[500][500][3];
  double m_Radius[500][500];
  double m_Size[500];
  float  m_Color[500][500][3];

  unsigned int  m_NumberOfPoints;
  unsigned int  m_NumberOfTubes;

  SurfaceTubeGlRenderMethod3D();

  ~SurfaceTubeGlRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  void BuildDisplayList();

  virtual void Update();

private:
  
  unsigned int  m_DisplayList;

};

} // end of namespace sov

#endif //__SurfaceTubeGlRenderMethod3D_h
