/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceTubeGlRenderMethod3DFactory.cxx,v $
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
#include <itkCreateObjectFunction.h>
#include <itkVersion.h>

#include <cstring>

#include "sovSurfaceTubeGlRenderMethod3D.h"
#include "sovSurfaceTubeGlRenderMethod3DFactory.h"

namespace sov
{
  void 
  SurfaceTubeGlRenderMethod3DFactory
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

  SurfaceTubeGlRenderMethod3DFactory
  ::SurfaceTubeGlRenderMethod3DFactory()
  {
    this->RegisterOverride("itkObject",
                           "SurfaceTubeGlRenderMethod3D",
                           "TubeSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<SurfaceTubeGlRenderMethod3D>::New());
  }

  SurfaceTubeGlRenderMethod3DFactory
  ::~SurfaceTubeGlRenderMethod3DFactory()
  {
  }

  const char *
  SurfaceTubeGlRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  SurfaceTubeGlRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create SurfaceTubeGlRenderMethod3D for the visualization of TubeSpatialObject classes";
  }

  LightObject::Pointer 
  SurfaceTubeGlRenderMethod3DFactory
  ::CreateObject( const char * itkclassname )
  {
    std::string name = itkclassname; 

    if( name == "SurfaceTubeGlRenderMethod3D" || name == "RenderMethod" )
      {
      LightObject::Pointer p = SurfaceTubeGlRenderMethod3D::New();
      return p;
    }
    else
    {
      return 0;
      }
  }

}
