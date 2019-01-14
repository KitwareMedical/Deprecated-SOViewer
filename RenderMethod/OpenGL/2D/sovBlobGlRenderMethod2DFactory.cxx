/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobGlRenderMethod2DFactory.cxx,v $
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

#include "sovBlobGlRenderMethod2D.h"
#include "sovBlobGlRenderMethod2DFactory.h"

namespace sov
{
  void 
  BlobGlRenderMethod2DFactory
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

  BlobGlRenderMethod2DFactory
  ::BlobGlRenderMethod2DFactory()
  {
    this->RegisterOverride("BlobSpatialObject",
                           "BlobGlRenderMethod2D",
                           "BlobSpatialObject as a 2D Render Method",
                           1,
                           CreateObjectFunction<BlobGlRenderMethod2D>::New());
  }

  BlobGlRenderMethod2DFactory
  ::~BlobGlRenderMethod2DFactory()
  {
  }

  const char *
  BlobGlRenderMethod2DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  BlobGlRenderMethod2DFactory
  ::GetDescription() const
  {
    return "Create BlobOpenGLRenderMethod2D for the visualization of BlobSpatialObject classes";
  }

}
