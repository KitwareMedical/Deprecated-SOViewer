/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBoxVTKRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2005-04-28 14:42:03 $
  Version:   $Revision: 1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _BoxVTKRenderMethod3D_h
#define _BoxVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include <itkBoxSpatialObject.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT BoxVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef BoxVTKRenderMethod3D     Self;
  typedef VTKRenderMethod3D           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef itk::BoxSpatialObject<3>  RectangleType;
  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;

  itkNewMacro( BoxVTKRenderMethod3D );
  itkTypeMacro( BoxVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  BoxVTKRenderMethod3D( void );
  ~BoxVTKRenderMethod3D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  virtual void Update();
};

}

#endif //_BoxVTKRenderMethod3D_h
