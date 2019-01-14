/*=========================================================================

  Program:   SOViewer
  Module:    $RCSfile: sovAttributeMeshVTKRenderMethod3DFactory.txx,v $
  Language:  C++
  Date:      $Date: 2005-12-15 19:46:11 $
  Version:   $Revision: 1.1 $
  Author:    Ipek Oguz

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef sovAttributeMeshVTKRenderMethod3DFactory_h
#define sovAttributeMeshVTKRenderMethod3DFactory_h

#include <itkVersion.h>

#include <cstring>

#include "sovAttributeMeshVTKRenderMethod3D.h"
#include "sovAttributeMeshVTKRenderMethod3DFactory.h"

template <class TMeshType>
void sov::AttributeMeshVTKRenderMethod3DFactory<TMeshType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent <<"Current autoload path: ";
  const char * path = getenv("ITK_AUTOLOAD_PATH");
  if( path )
  {
    os<<path<<std::endl;
  }
  else
  {
    os<<"NULL"<<std::endl;
  }
}

template < class TMeshType>
sov::AttributeMeshVTKRenderMethod3DFactory<TMeshType>
::AttributeMeshVTKRenderMethod3DFactory()
{
  typedef AttributeMeshVTKRenderMethod3D<TMeshType> AttributeMeshVTKRenderMethod3DType;
  this->RegisterOverride("AttributeMeshSpatialObject",
    "AttributeMeshVTKRenderMethod3D",
    "AttributeMeshSpatialObject Point Render Method",
    1,
    CreateObjectFunction<AttributeMeshVTKRenderMethod3DType>::New());
  this->RegisterOverride("MeshSpatialObject",
    "AttributeMeshVTKRenderMethod3D",
    "AttributeMeshSpatialObject Point Render Method",
    1,
    CreateObjectFunction<AttributeMeshVTKRenderMethod3DType>::New());
}

template < class TMeshType>
sov::AttributeMeshVTKRenderMethod3DFactory<TMeshType>
::~AttributeMeshVTKRenderMethod3DFactory()
{
}

template < class TMeshType >
const char *
sov::AttributeMeshVTKRenderMethod3DFactory<TMeshType>
::GetITKSourceVersion() const
{
  return ITK_SOURCE_VERSION;
}

template < class TMeshType >
const char *
sov::AttributeMeshVTKRenderMethod3DFactory<TMeshType>
::GetDescription() const
{
  return "Create AttributeMeshVTKRenderMethod3D for the visualization of AttributeMeshSpatialObject classes";
}

#endif

