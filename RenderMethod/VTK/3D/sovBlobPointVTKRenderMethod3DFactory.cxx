/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobPointVTKRenderMethod3DFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-15 00:27:42 $
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

#include "sovBlobPointVTKRenderMethod3D.h"
#include "sovBlobPointVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  BlobPointVTKRenderMethod3DFactory
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

  BlobPointVTKRenderMethod3DFactory
  ::BlobPointVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("BlobSpatialObject",
                           "BlobPointVTKRenderMethod3D",
                           "BlobSpatialObject Point Render Method",
                           1,
                           CreateObjectFunction<BlobPointVTKRenderMethod3D>::New());
  }

  BlobPointVTKRenderMethod3DFactory
  ::~BlobPointVTKRenderMethod3DFactory()
  {
  }

  const char *
  BlobPointVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  BlobPointVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create BlobPointVTKRenderMethod3D for the visualization of BlobSpatialObject classes";
  }
}
