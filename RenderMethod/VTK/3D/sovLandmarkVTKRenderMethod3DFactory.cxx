/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLandmarkVTKRenderMethod3DFactory.cxx,v $
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

#include "sovLandmarkVTKRenderMethod3D.h"
#include "sovLandmarkVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  LandmarkVTKRenderMethod3DFactory
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

  LandmarkVTKRenderMethod3DFactory
  ::LandmarkVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("LandmarkSpatialObject",
                           "LandmarkVTKRenderMethod3D",
                           "LandmarkSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<LandmarkVTKRenderMethod3D>::New());
  }

  LandmarkVTKRenderMethod3DFactory
  ::~LandmarkVTKRenderMethod3DFactory()
  {
  }

  const char *
  LandmarkVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  LandmarkVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create LandmarkVTKRenderMethod3D for the visualization of EllipseSpatialObject classes";
  }
}
