/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovTubeGlRenderMethod3DFactory.cxx,v $
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

#include "sovTubeGlRenderMethod3DFactory.h"
#include "sovTubeGlRenderMethod3D.h"

namespace sov
{
  void 
  TubeGlRenderMethod3DFactory
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

  TubeGlRenderMethod3DFactory
  ::TubeGlRenderMethod3DFactory()
  {
    this->RegisterOverride("itkObject",
                           "TubeGlRenderMethod3D",
                           "TubeSpatialObject Render Method",
                           1,
                           CreateObjectFunction<TubeGlRenderMethod3D>::New());
  }

  TubeGlRenderMethod3DFactory
  ::~TubeGlRenderMethod3DFactory()
  {
  }

  const char *
  TubeGlRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  TubeGlRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create TubeGlRenderMethod3DFactory for the visualization of TubeSpatialObject classes";
  }

  LightObject::Pointer 
  TubeGlRenderMethod3DFactory
  ::CreateObject( const char * itkclassname )
  {
    //std::cout<<"TubeOpenGLRenderMethodFactory::CreateObject()"<<std::endl;

    std::string name = itkclassname; 
    
    if( name == "TubeGlRenderMethod" || name == "RenderMethod" )
      {
      LightObject::Pointer p = TubeGlRenderMethod3D::New();
      return p;
      }
    else
      {
      return 0;
      }
  }
  /**/

} //end of namespace sov
