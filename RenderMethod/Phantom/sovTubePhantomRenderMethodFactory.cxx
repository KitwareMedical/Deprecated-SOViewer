/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovTubePhantomRenderMethodFactory.cxx,v $
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

#include "sovTubePhantomRenderMethod.h"
#include "sovTubePhantomRenderMethodFactory.h"

namespace sov
{
  void 
  TubePhantomRenderMethodFactory
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

  TubePhantomRenderMethodFactory
  ::TubePhantomRenderMethodFactory()
  {
    this->RegisterOverride("TubeSpatialObject",
                           "TubePhantomRenderMethod",
                           "TubeSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<TubePhantomRenderMethod>::New());
  }

  TubePhantomRenderMethodFactory
  ::~TubePhantomRenderMethodFactory()
  {
  }

  const char *
  TubePhantomRenderMethodFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  TubePhantomRenderMethodFactory
  ::GetDescription() const
  {
    return "Create TubePhantomRenderMethod for the visualization of TubeSpatialObject classes";
  }

}
