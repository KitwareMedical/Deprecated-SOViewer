/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSparsePointVTKRenderMethod3DFactory.cxx,v $
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

#include "sovSparsePointVTKRenderMethod3D.h"
#include "sovSparsePointVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  SparsePointVTKRenderMethod3DFactory
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

  SparsePointVTKRenderMethod3DFactory
  ::SparsePointVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("SparsePointsSpatialObject",
                           "SparsePointVTKRenderMethod3D",
                           "SparsePointsSpatialObject Render Method",
                           1,
                           CreateObjectFunction<SparsePointVTKRenderMethod3D>::New());
  }

  SparsePointVTKRenderMethod3DFactory
  ::~SparsePointVTKRenderMethod3DFactory()
  {
  }

  const char *
  SparsePointVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  SparsePointVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create SparsePointVTKRenderMethod3D for the visualization of SparsePointsSpatialObject classes";
  }
}
