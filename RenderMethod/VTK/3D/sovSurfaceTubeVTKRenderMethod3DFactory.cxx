/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceTubeVTKRenderMethod3DFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2005-06-27 16:23:23 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <itkVersion.h>

#include <cstring>

#include "sovSurfaceTubeVTKRenderMethod3D.h"
#include "sovSurfaceTubeVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  SurfaceTubeVTKRenderMethod3DFactory
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

  SurfaceTubeVTKRenderMethod3DFactory
  ::SurfaceTubeVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("TubeSpatialObject",
                           "SurfaceTubeVTKRenderMethod3D",
                           "Color TubeSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<SurfaceTubeVTKRenderMethod3D>::New());
    this->RegisterOverride("VesselTubeSpatialObject",
                           "SurfaceTubeVTKRenderMethod3D",
                           "Color TubeSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<SurfaceTubeVTKRenderMethod3D>::New());
    this->RegisterOverride("DTITubeSpatialObject",
                           "SurfaceTubeVTKRenderMethod3D",
                           "Color TubeSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<SurfaceTubeVTKRenderMethod3D>::New());
  }

  SurfaceTubeVTKRenderMethod3DFactory
  ::~SurfaceTubeVTKRenderMethod3DFactory()
  {
  }

  const char *
  SurfaceTubeVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  SurfaceTubeVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create SurfaceTubeVTKRenderMethod3D for the visualization of TubeSpatialObject classes";
  }

}
