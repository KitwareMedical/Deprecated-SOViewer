/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLandmarkVTKRenderMethod3D.h,v $
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
#ifndef _LandmarkVTKRenderMethod3D_h
#define _LandmarkVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include <itkLandmarkSpatialObject.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT LandmarkVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef LandmarkVTKRenderMethod3D     Self;
  typedef VTKRenderMethod3D             Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;
  typedef itk::LandmarkSpatialObject<3> LandmarkType;
  typedef LandmarkType::PointListType   LandmarkPointListType;
  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;

  itkNewMacro( LandmarkVTKRenderMethod3D );
  itkTypeMacro( LandmarkVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  LandmarkVTKRenderMethod3D( void );
  ~LandmarkVTKRenderMethod3D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  virtual void Update();
};

}

#endif //_LandmarkVTKRenderMethod3D_h
