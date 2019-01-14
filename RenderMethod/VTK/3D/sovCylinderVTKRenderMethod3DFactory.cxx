/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovCylinderVTKRenderMethod3DFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-12 22:28:00 $
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

#include "sovCylinderVTKRenderMethod3D.h"
#include "sovCylinderVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  CylinderVTKRenderMethod3DFactory
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

  CylinderVTKRenderMethod3DFactory
  ::CylinderVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("CylinderSpatialObject",
                           "CylinderVTKRenderMethod3D",
                           "CylinderSpatialObject Render Method",
                           1,
                           CreateObjectFunction<CylinderVTKRenderMethod3D>::New());
  }

  CylinderVTKRenderMethod3DFactory
  ::~CylinderVTKRenderMethod3DFactory()
  {
  }

  const char *
  CylinderVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  CylinderVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create CylinderVTKRenderMethod3D for the visualization of RectangleSpatialObject classes";
  }
}
