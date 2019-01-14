/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobSurfaceVTKRenderMethod3DFactory.cxx,v $
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

#include "sovBlobSurfaceVTKRenderMethod3D.h"
#include "sovBlobSurfaceVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  BlobSurfaceVTKRenderMethod3DFactory
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

  BlobSurfaceVTKRenderMethod3DFactory
  ::BlobSurfaceVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("BlobSpatialObject",
                           "BlobSurfaceVTKRenderMethod3D",
                           "BlobSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<BlobSurfaceVTKRenderMethod3D>::New());
  }

  BlobSurfaceVTKRenderMethod3DFactory
  ::~BlobSurfaceVTKRenderMethod3DFactory()
  {
  }

  const char *
  BlobSurfaceVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  BlobSurfaceVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create BlobSurfaceVTKRenderMethod3D for the visualization of BlobSpatialObject classes";
  }
}
