/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceMeshVTKRenderMethod3DFactory.txx,v $
  Language:  C++
  Date:      $Date: 2005-02-02 15:26:35 $
  Version:   $Revision: 1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <itkVersion.h>

#include <cstring>

#include "sovSurfaceMeshVTKRenderMethod3D.h"
#include "sovSurfaceMeshVTKRenderMethod3DFactory.h"

namespace sov
{
  template <class MeshType>
  void 
  SurfaceMeshVTKRenderMethod3DFactory<MeshType>
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

  template <class MeshType>
  SurfaceMeshVTKRenderMethod3DFactory<MeshType>
  ::SurfaceMeshVTKRenderMethod3DFactory()
  {
    typedef SurfaceMeshVTKRenderMethod3D<MeshType> SurfaceMeshVTKRenderMethod3DType;
    this->RegisterOverride("MeshSpatialObject",
                           "SurfaceMeshVTKRenderMethod3D",
                           "MeshSpatialObject Point Render Method",
                           1,
                           CreateObjectFunction<SurfaceMeshVTKRenderMethod3DType>::New());
  }

  template <class MeshType>
  SurfaceMeshVTKRenderMethod3DFactory<MeshType>
  ::~SurfaceMeshVTKRenderMethod3DFactory()
  {
  }

  template <class MeshType>
  const char *
  SurfaceMeshVTKRenderMethod3DFactory<MeshType>
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  template <class MeshType>
  const char *
  SurfaceMeshVTKRenderMethod3DFactory<MeshType>
  ::GetDescription() const
  {
    return "Create SurfaceMeshVTKRenderMethod3D for the visualization of EllipseSpatialObject classes";
  }
}
