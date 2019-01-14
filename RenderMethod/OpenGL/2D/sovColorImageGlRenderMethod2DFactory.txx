/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovColorImageGlRenderMethod2DFactory.txx,v $
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

#include "sovColorImageGlRenderMethod2D.h"
#include "sovColorImageGlRenderMethod2DFactory.h"

namespace sov
{
  template <class ImagePixelType>
  void 
  ColorImageGlRenderMethod2DFactory<ImagePixelType>
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

  template <class ImagePixelType>
  ColorImageGlRenderMethod2DFactory<ImagePixelType>
  ::ColorImageGlRenderMethod2DFactory()
  {
    typedef ColorImageGlRenderMethod2D<ImageType> ColorImageGlRenderMethodType;
    this->RegisterOverride("ImageSpatialObject",
                           "ColorImageGlRenderMethod2D",
                           "ImageSpatialObject as one view of a slice Render Method",
                           1,
                           CreateObjectFunction<ColorImageGlRenderMethodType>::New());
  }

  template <class ImagePixelType>
  ColorImageGlRenderMethod2DFactory<ImagePixelType>
  ::~ColorImageGlRenderMethod2DFactory()
  {
  }

  template <class ImagePixelType>
  const char *
  ColorImageGlRenderMethod2DFactory<ImagePixelType>
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  template <class ImagePixelType>
  const char *
  ColorImageGlRenderMethod2DFactory<ImagePixelType>
  ::GetDescription() const
  {
    return "Create ImageVTKRenderMethod3D for the visualization of ImageSpatialObject classes";
  }

}
