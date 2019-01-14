/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceNormalVectorVTKRenderMethod3DFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2005-04-25 14:37:45 $
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
#include "sovSurfaceNormalVectorVTKRenderMethod3D.h"
#include "sovSurfaceNormalVectorVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  SurfaceNormalVectorVTKRenderMethod3DFactory
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

  SurfaceNormalVectorVTKRenderMethod3DFactory
  ::SurfaceNormalVectorVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("SurfaceSpatialObject",
                           "SurfaceNormalVectorVTKRenderMethod3D",
                           "SurfaceSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<SurfaceNormalVectorVTKRenderMethod3D>::New());
  }

  SurfaceNormalVectorVTKRenderMethod3DFactory
  ::~SurfaceNormalVectorVTKRenderMethod3DFactory()
  {
  }

  const char *
  SurfaceNormalVectorVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  SurfaceNormalVectorVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create SurfaceNormalVectorVTKRenderMethod3D for the visualization of SurfaceSpatialObject classes";
  }
}
