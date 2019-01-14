/*=========================================================================

  Program:   SOViewer
  Module:    $RCSfile: sovAttributeMeshVTKRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2005-12-15 19:49:24 $
  Version:   $Revision: 1.2 $
  Author:    Ipek Oguz

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if !defined(sov__AttributeMeshRenderMethod3D__h)
#define sov__AttributeMeshRenderMethod3D__h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <sovSurfaceMeshVTKRenderMethod3D.h>
#include <vtkLoopSubdivisionFilter.h>
#include <sovColorScheme.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>
#include <vtkPointSource.h>
#include <vtkSphereSource.h>
#include <vtkCellType.h>
#include <vtkPoints.h>
#include <itkVariableLengthVector.h>

namespace sov 
{  
  template < class TMesh >
    class AttributeMeshVTKRenderMethod3D : public SurfaceMeshVTKRenderMethod3D <TMesh>
  {
    
public:
  typedef AttributeMeshVTKRenderMethod3D Self;
  typedef SurfaceMeshVTKRenderMethod3D<TMesh> Superclass;
  typedef SmartPointer< Self > Pointer;
  typedef SmartPointer< const Self > ConstPointer;
  typedef typename TMesh::PixelType PixelType;
  
  typedef TMesh                                MeshType;
  typedef typename Superclass::SurfaceMeshType SurfaceMeshType;
  typedef typename Superclass::ObjectType      ObjectType;
  typedef typename Superclass::ObjectListType  ObjectListType;
  
  itkNewMacro( AttributeMeshVTKRenderMethod3D );
  itkTypeMacro( AttributeMeshVTKRenderMethod3D, SurfaceMeshVTKRenderMethod3D );
  
  AttributeMeshVTKRenderMethod3D();
  virtual ~AttributeMeshVTKRenderMethod3D();
  
protected:
  void Update () ;
  };
}

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovAttributeMeshVTKRenderMethod3D.txx"
#endif

#endif
