/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovCylinderVTKRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2005-09-12 22:28:00 $
  Version:   $Revision: 1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _CylinderVTKRenderMethod3D_h
#define _CylinderVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include <itkCylinderSpatialObject.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT CylinderVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef CylinderVTKRenderMethod3D      Self;
  typedef VTKRenderMethod3D              Superclass;
  typedef SmartPointer< Self >           Pointer;
  typedef SmartPointer< const Self >     ConstPointer;
  typedef itk::CylinderSpatialObject     CylinderType;
  typedef Superclass::ObjectType         ObjectType;
  typedef Superclass::ObjectListType     ObjectListType;

  itkNewMacro( CylinderVTKRenderMethod3D );
  itkTypeMacro( CylinderVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  CylinderVTKRenderMethod3D( void );
  ~CylinderVTKRenderMethod3D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  virtual void Update();
};

}

#endif //_CylinderVTKRenderMethod3D_h
