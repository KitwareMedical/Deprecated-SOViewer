/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfacePointVTKRenderMethod3D.h,v $
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
#ifndef __SurfacePointVTKRenderMethod3D_h
#define __SurfacePointVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include "itkSurfaceSpatialObject.h"
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT SurfacePointVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef SurfacePointVTKRenderMethod3D       Self;
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

  itkNewMacro( SurfacePointVTKRenderMethod3D );

  itkTypeMacro( SurfacePointVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  SurfacePointVTKRenderMethod3D();

  ~SurfacePointVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

};

} // end of namespace sov

#endif //__SurfacePointVTKRenderMethod3D_h
