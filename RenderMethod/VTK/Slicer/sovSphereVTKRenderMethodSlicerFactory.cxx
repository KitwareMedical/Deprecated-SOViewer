/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSphereVTKRenderMethodSlicerFactory.cxx,v $
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
#include "sovSphereVTKRenderMethodSlicer.h"
#include "sovSphereVTKRenderMethodSlicerFactory.h"

namespace sov
{
  void 
  SphereVTKRenderMethodSlicerFactory
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

  SphereVTKRenderMethodSlicerFactory
  ::SphereVTKRenderMethodSlicerFactory()
  {
    this->RegisterOverride("EllipseSpatialObject",
                           "SphereVTKRenderMethodSlicer",
                           "EllipseSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<SphereVTKRenderMethodSlicer>::New());
  }

  SphereVTKRenderMethodSlicerFactory
  ::~SphereVTKRenderMethodSlicerFactory()
  {
  }

  const char *
  SphereVTKRenderMethodSlicerFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  SphereVTKRenderMethodSlicerFactory
  ::GetDescription() const
  {
    return "Create SphereVTKRenderMethodSlicer for the visualization of EllipseSpatialObject classes";
  }
}
