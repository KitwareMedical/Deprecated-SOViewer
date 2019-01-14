/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovWireframeMeshVTKRenderMethod3DFactory.txx,v $
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
#include <itkVersion.h>

#include <cstring>

#include "sovWireframeMeshVTKRenderMethod3D.h"
#include "sovWireframeMeshVTKRenderMethod3DFactory.h"

namespace sov
{
  template <class MeshType>
  void 
  WireframeMeshVTKRenderMethod3DFactory<MeshType>
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
  WireframeMeshVTKRenderMethod3DFactory<MeshType>
  ::WireframeMeshVTKRenderMethod3DFactory()
  {
    typedef WireframeMeshVTKRenderMethod3D<MeshType> WireframeMeshVTKRenderMethodType;
    
    this->RegisterOverride("MeshSpatialObject",
                           "WireframeMeshVTKRenderMethod3D",
                           "MeshSpatialObject wireframe Render Method",
                           1,
                           CreateObjectFunction<WireframeMeshVTKRenderMethodType>::New());
  }

  template <class MeshType>
  WireframeMeshVTKRenderMethod3DFactory<MeshType>
  ::~WireframeMeshVTKRenderMethod3DFactory()
  {
  }

  template <class MeshType>
  const char *
  WireframeMeshVTKRenderMethod3DFactory<MeshType>
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  template <class MeshType>
  const char *
  WireframeMeshVTKRenderMethod3DFactory<MeshType>
  ::GetDescription() const
  {
    return "Create WireframeMeshVTKRenderMethod3D for the visualization of EllipseSpatialObject classes";
  }
}
