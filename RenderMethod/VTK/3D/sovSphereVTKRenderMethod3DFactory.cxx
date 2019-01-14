/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSphereVTKRenderMethod3DFactory.cxx,v $
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
#include "sovSphereVTKRenderMethod3D.h"
#include "sovSphereVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  SphereVTKRenderMethod3DFactory
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

  SphereVTKRenderMethod3DFactory
  ::SphereVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("EllipseSpatialObject",
                           "SphereVTKRenderMethod3D",
                           "EllipseSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<SphereVTKRenderMethod3D>::New());
  }

  SphereVTKRenderMethod3DFactory
  ::~SphereVTKRenderMethod3DFactory()
  {
  }

  const char *
  SphereVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  SphereVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create SphereVTKRenderMethod3D for the visualization of EllipseSpatialObject classes";
  }
}
