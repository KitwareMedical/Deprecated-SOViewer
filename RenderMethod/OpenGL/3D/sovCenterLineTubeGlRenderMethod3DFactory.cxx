/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovCenterLineTubeGlRenderMethod3DFactory.cxx,v $
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

#include "sovCenterLineTubeGlRenderMethod3DFactory.h"
#include "sovGlRenderMethod3D.h"

namespace sov
{
  void 
  CenterLineTubeGlRenderMethod3DFactory
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

  CenterLineTubeGlRenderMethod3DFactory
  ::CenterLineTubeGlRenderMethod3DFactory()
  {
    this->RegisterOverride("itkObject",
                           "CenterLineTubeGlRenderMethod3D",
                           "SpatialObject OpenGL Render Method",
                           1,
                           CreateObjectFunction<CenterLineTubeGlRenderMethod3D>::New());
  }

  CenterLineTubeGlRenderMethod3DFactory
  ::~CenterLineTubeGlRenderMethod3DFactory()
  {
  }

  const char *
  CenterLineTubeGlRenderMethod3DFactory
  ::GetITKSourceVersion( void ) const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  CenterLineTubeGlRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create SORenderMethod for the visualization of SpatialObject classes";
  }

  LightObject::Pointer 
  CenterLineTubeGlRenderMethod3DFactory
  ::CreateObject( const char * itkclassname )
  {
    std::string name = itkclassname; 

    if( name == "CenterLineTubeGlRenderMethod3D" || name == "RenderMethod" )
    {
      //LightObject::Pointer p = CenterLineTubeGlRenderMethod3D::New();
      return CenterLineTubeGlRenderMethod3D::New().GetPointer();
    }
    else
    {
      return 0;
    }
  }
}
