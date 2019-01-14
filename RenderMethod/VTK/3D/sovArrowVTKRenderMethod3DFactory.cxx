/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovArrowVTKRenderMethod3DFactory.cxx,v $
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

#include "sovArrowVTKRenderMethod3D.h"
#include "sovArrowVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  ArrowVTKRenderMethod3DFactory
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

  ArrowVTKRenderMethod3DFactory
  ::ArrowVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("ArrowSpatialObject",
                           "ArrowVTKRenderMethod3D",
                           "ArrowSpatialObject Render Method",
                           1,
                           CreateObjectFunction<ArrowVTKRenderMethod3D>::New());
  }

  ArrowVTKRenderMethod3DFactory
  ::~ArrowVTKRenderMethod3DFactory()
  {
  }

  const char *
  ArrowVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  ArrowVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create ArrowVTKRenderMethod3D for the visualization of ArrowSpatialObject classes";
  }
}
