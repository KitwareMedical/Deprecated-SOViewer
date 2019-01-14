/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovEllipseGlRenderMethod2DFactory.cxx,v $
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

#include "sovEllipseGlRenderMethod2D.h"
#include "sovEllipseGlRenderMethod2DFactory.h"

namespace sov
{
  void 
  EllipseGlRenderMethod2DFactory
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

  EllipseGlRenderMethod2DFactory
  ::EllipseGlRenderMethod2DFactory()
  {
    this->RegisterOverride("EllipseSpatialObject",
                           "EllipseGlRenderMethod2D",
                           "EllipseSpatialObject as a 2D Render Method",
                           1,
                           CreateObjectFunction<EllipseGlRenderMethod2D>::New());
  }

  EllipseGlRenderMethod2DFactory
  ::~EllipseGlRenderMethod2DFactory()
  {
  }

  const char *
  EllipseGlRenderMethod2DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  EllipseGlRenderMethod2DFactory
  ::GetDescription() const
  {
    return "Create EllipseOpenGLRenderMethod2D for the visualization of EllipseSpatialObject classes";
  }

}
