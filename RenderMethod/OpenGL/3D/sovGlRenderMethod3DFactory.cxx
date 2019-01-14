/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderMethod3DFactory.cxx,v $
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

#include "sovGlRenderMethod3DFactory.h"
#include "sovGlRenderMethod3D.h"

namespace sov
{
  void 
  GlRenderMethod3DFactory
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

  GlRenderMethod3DFactory
  ::GlRenderMethod3DFactory()
  {
    this->RegisterOverride("itkObject",
                           "GlRenderMethod3D",
                           "SpatialObject OpenGL Render Method",
                           1,
                           CreateObjectFunction<GlRenderMethod3D>::New());
  }

  GlRenderMethod3DFactory
  ::~GlRenderMethod3DFactory()
  {
  }

  const char *
  GlRenderMethod3DFactory
  ::GetITKSourceVersion( void ) const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  GlRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create SORenderMethod for the visualization of SpatialObject classes";
  }

  LightObject::Pointer 
  GlRenderMethod3DFactory
  ::CreateObject( const char * itkclassname )
  {
    std::string name = itkclassname; 

    if( name == "GlRenderMethod3D" || name == "RenderMethod" )
      {
      LightObject::Pointer p = GlRenderMethod3D::New();
      return p;
      }
    else
      {
      return 0;
      }
  }
}
