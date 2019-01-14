/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovArrowVTKRenderMethod3D.h,v $
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
#ifndef _ArrowVTKRenderMethod3D_h
#define _ArrowVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include <itkArrowSpatialObject.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT ArrowVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef ArrowVTKRenderMethod3D     Self;
  typedef VTKRenderMethod3D           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef itk::ArrowSpatialObject<3>  ArrowType;
  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;

  itkNewMacro( ArrowVTKRenderMethod3D );
  itkTypeMacro( ArrowVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  ArrowVTKRenderMethod3D( void );
  ~ArrowVTKRenderMethod3D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  virtual void Update();
};

}

#endif //_ArrowVTKRenderMethod3D_h
