/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceTubeGlRenderMethodSlicerFactory.cxx,v $
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

#include "sovSurfaceTubeGlRenderMethodSlicer.h"
#include "sovSurfaceTubeGlRenderMethodSlicerFactory.h"

namespace sov
{

  void 
  SurfaceTubeGlRenderMethodSlicerFactory
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


  SurfaceTubeGlRenderMethodSlicerFactory
  ::SurfaceTubeGlRenderMethodSlicerFactory()
  {
    this->RegisterOverride("TubeSpatialObject",
                           "SurfaceTubeGlRenderMethodSlicer",
                           "TubeSpatialObject as one view of a slice Render Method",
                           1,
                           CreateObjectFunction<SurfaceTubeGlRenderMethodSlicer>::New());
  }


  SurfaceTubeGlRenderMethodSlicerFactory
  ::~SurfaceTubeGlRenderMethodSlicerFactory()
  {
  }


  const char *
  SurfaceTubeGlRenderMethodSlicerFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }


  const char *
  SurfaceTubeGlRenderMethodSlicerFactory
  ::GetDescription() const
  {
    return "Create ImageVTKRenderMethod3D for the visualization of ImageSpatialObject classes";
  }

}
