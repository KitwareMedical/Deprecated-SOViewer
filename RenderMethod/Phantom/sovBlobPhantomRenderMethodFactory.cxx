/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobPhantomRenderMethodFactory.cxx,v $
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

#include "sovBlobPhantomRenderMethod.h"
#include "sovBlobPhantomRenderMethodFactory.h"

namespace sov
{
  void 
  BlobPhantomRenderMethodFactory
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

  BlobPhantomRenderMethodFactory
  ::BlobPhantomRenderMethodFactory()
  {
    this->RegisterOverride("BlobSpatialObject",
                           "BlobPhantomRenderMethod",
                           "BlobSpatialObject surface Render Method",
                           1,
                           CreateObjectFunction<BlobPhantomRenderMethod>::New());
  }

  BlobPhantomRenderMethodFactory
  ::~BlobPhantomRenderMethodFactory()
  {
  }

  const char *
  BlobPhantomRenderMethodFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  BlobPhantomRenderMethodFactory
  ::GetDescription() const
  {
    return "Create BlobPhantomRenderMethod for the visualization of BlobSpatialObject classes";
  }
}
