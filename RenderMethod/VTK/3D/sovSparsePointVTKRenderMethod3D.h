/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSparsePointVTKRenderMethod3D.h,v $
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
#ifndef _SparsePointVTKRenderMethod3D_h
#define _SparsePointVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include <itkSparsePointsSpatialObject.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT SparsePointVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef SparsePointVTKRenderMethod3D     Self;
  typedef VTKRenderMethod3D           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef itk::SparsePointsSpatialObject<3>  SparsePointType;
  typedef SparsePointType::SparsePointListType SparsePointListType;
  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;

  itkNewMacro( SparsePointVTKRenderMethod3D );
  itkTypeMacro( SparsePointVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  SparsePointVTKRenderMethod3D( void );
  ~SparsePointVTKRenderMethod3D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  virtual void Update();
};

}

#endif //_SparsePointVTKRenderMethod3D_h
