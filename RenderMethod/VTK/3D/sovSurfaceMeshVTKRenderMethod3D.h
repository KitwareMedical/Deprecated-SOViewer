/*=========================================================================
  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceMeshVTKRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2005-12-15 19:49:07 $
  Version:   $Revision: 1.4 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _SurfaceMeshVTKRenderMethod3D_h
#define _SurfaceMeshVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"

#include "vtkPolyData.h"
#include <itkMeshSpatialObject.h>
#include <vtkIdList.h>

namespace sov
{

template <class TMesh>
class SurfaceMeshVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef SurfaceMeshVTKRenderMethod3D     Self;
  typedef VTKRenderMethod3D           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;

  typedef TMesh MeshType;
  typedef typename TMesh::PixelType PixelType;
  typedef itk::MeshSpatialObject<MeshType>    SurfaceMeshType;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  itkNewMacro( SurfaceMeshVTKRenderMethod3D );
  itkTypeMacro( SurfaceMeshVTKRenderMethod3D, VTKRenderMethod3D );

protected:

  SurfaceMeshVTKRenderMethod3D( void );
  ~SurfaceMeshVTKRenderMethod3D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  virtual void Update();
};

}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovSurfaceMeshVTKRenderMethod3D.txx"
#endif

#endif //_SurfaceMeshVTKRenderMethod3D_h
