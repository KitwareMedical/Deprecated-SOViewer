/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLandmarkGlRenderMethodSlicerFactory.cxx,v $
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

#include "sovLandmarkGlRenderMethodSlicer.h"
#include "sovLandmarkGlRenderMethodSlicerFactory.h"

namespace sov
{

  void 
  LandmarkGlRenderMethodSlicerFactory
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


  LandmarkGlRenderMethodSlicerFactory
  ::LandmarkGlRenderMethodSlicerFactory()
  {
    this->RegisterOverride("LandmarkSpatialObject",
                           "LandmarkGlRenderMethodSlicer",
                           "LandmarkSpatialObject as cross on a slice Render Method",
                           1,
                           CreateObjectFunction<LandmarkGlRenderMethodSlicer>::New());
  }


  LandmarkGlRenderMethodSlicerFactory
  ::~LandmarkGlRenderMethodSlicerFactory()
  {
  }


  const char *
  LandmarkGlRenderMethodSlicerFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }


  const char *
  LandmarkGlRenderMethodSlicerFactory
  ::GetDescription() const
  {
    return "Create LandmarkGlRenderMethodSlicer for the visualization of landmark within a slicer renderer.";
  }

}
