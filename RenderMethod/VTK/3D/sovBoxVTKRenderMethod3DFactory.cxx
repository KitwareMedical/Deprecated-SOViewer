/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBoxVTKRenderMethod3DFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2005-04-28 14:42:03 $
  Version:   $Revision: 1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <itkVersion.h>

#include <cstring>

#include "sovBoxVTKRenderMethod3D.h"
#include "sovBoxVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  BoxVTKRenderMethod3DFactory
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

  BoxVTKRenderMethod3DFactory
  ::BoxVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("BoxSpatialObject",
                           "BoxVTKRenderMethod3D",
                           "BoxSpatialObject Render Method",
                           1,
                           CreateObjectFunction<BoxVTKRenderMethod3D>::New());
  }

  BoxVTKRenderMethod3DFactory
  ::~BoxVTKRenderMethod3DFactory()
  {
  }

  const char *
  BoxVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  BoxVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create BoxVTKRenderMethod3D for the visualization of RectangleSpatialObject classes";
  }
}
