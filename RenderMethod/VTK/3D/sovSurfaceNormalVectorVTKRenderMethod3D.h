/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceNormalVectorVTKRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2005-04-25 14:37:45 $
  Version:   $Revision: 1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __SurfaceNormalVectorVTKRenderMethod3D_h
#define __SurfaceNormalVectorVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include "itkSurfaceSpatialObject.h"
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT SurfaceNormalVectorVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef SurfaceNormalVectorVTKRenderMethod3D       Self;
  typedef SmartPointer< Self >                Pointer;
  typedef VTKRenderMethod3D                   Superclass;
  
  typedef itk::SurfaceSpatialObject<3>        SurfaceType;
  typedef SurfaceType::Pointer                SurfacePointer;
  typedef SurfaceType::SurfacePointType       SurfacePointType;
  typedef SurfacePointType*                   SurfacePointPointer;
  typedef SurfaceType::PointListType          SurfacePointListType;
  typedef SurfacePointListType *              SurfacePointListPointer;
  typedef SurfacePointType::PointType         PointType;
  typedef PointType *                         PointPointer;

  itkNewMacro( SurfaceNormalVectorVTKRenderMethod3D );

  itkTypeMacro( SurfaceNormalVectorVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  SurfaceNormalVectorVTKRenderMethod3D();

  ~SurfaceNormalVectorVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

};

} // end of namespace sov

#endif //__SurfaceNormalVectorVTKRenderMethod3D_h
