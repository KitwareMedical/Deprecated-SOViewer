/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSpherePhantomRenderMethodFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:58 $
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

#include "sovSpherePhantomRenderMethod.h"
#include "sovSpherePhantomRenderMethodFactory.h"

namespace sov
{
  void 
  SpherePhantomRenderMethodFactory
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

  SpherePhantomRenderMethodFactory
  ::SpherePhantomRenderMethodFactory()
  {
    this->RegisterOverride("EllipseSpatialObject",
                           "SpherePhantomRenderMethod",
                           "EllipseSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<SpherePhantomRenderMethod>::New());
  }

  SpherePhantomRenderMethodFactory
  ::~SpherePhantomRenderMethodFactory()
  {
  }

  const char *
  SpherePhantomRenderMethodFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  SpherePhantomRenderMethodFactory
  ::GetDescription() const
  {
    return "Create SpherePhantomRenderMethod for the visualization of EllipseSpatialObject classes";
  }
}
