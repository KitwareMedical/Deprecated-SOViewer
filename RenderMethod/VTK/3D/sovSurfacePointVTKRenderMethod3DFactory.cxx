/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfacePointVTKRenderMethod3DFactory.cxx,v $
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
#include "sovSurfacePointVTKRenderMethod3D.h"
#include "sovSurfacePointVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  SurfacePointVTKRenderMethod3DFactory
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

  SurfacePointVTKRenderMethod3DFactory
  ::SurfacePointVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("SurfaceSpatialObject",
                           "SurfacePointVTKRenderMethod3D",
                           "SurfaceSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<SurfacePointVTKRenderMethod3D>::New());
  }

  SurfacePointVTKRenderMethod3DFactory
  ::~SurfacePointVTKRenderMethod3DFactory()
  {
  }

  const char *
  SurfacePointVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  SurfacePointVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create SurfacePointVTKRenderMethod3D for the visualization of SurfaceSpatialObject classes";
  }
}
