/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovWireframeMeshVTKRenderMethod3D.h,v $
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
#ifndef _WireframeMeshVTKRenderMethod3D_h
#define _WireframeMeshVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"

#include <itkMeshSpatialObject.h>
#include <itkDefaultDynamicMeshTraits.h>


#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif

namespace sov
{

template <class TMesh>
class WireframeMeshVTKRenderMethod3D
: public VTKRenderMethod3D
{

public:

  typedef WireframeMeshVTKRenderMethod3D     Self;
  typedef VTKRenderMethod3D           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;

  typedef TMesh MeshType;

  typedef typename MeshType::MeshTraits MeshTrait;
  typedef itk::MeshSpatialObject<MeshType>    WireframeMeshType;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  itkNewMacro( WireframeMeshVTKRenderMethod3D );
  itkTypeMacro( WireframeMeshVTKRenderMethod3D, VTKRenderMethod3D );

protected:

  WireframeMeshVTKRenderMethod3D( void );
  ~WireframeMeshVTKRenderMethod3D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  virtual void Update();

  struct pair
  {
    vtkFloatingPointType x[3];
    vtkFloatingPointType y[3];
  };

  std::list<pair> m_LineList;

  bool IsInLineList(pair * l);
};

}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovWireframeMeshVTKRenderMethod3D.txx"
#endif

#endif //_WireframeMeshVTKRenderMethod3D_h
