/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageVTKRenderMethodSlicerFactory.txx,v $
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

#include "sovImageVTKRenderMethodSlicer.h"
#include "sovImageVTKRenderMethodSlicerFactory.h"

namespace sov
{
  template <class ImagePixelType>
  void 
  ImageVTKRenderMethodSlicerFactory<ImagePixelType>
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
  ImageVTKRenderMethodSlicerFactory<ImagePixelType>
  ::ImageVTKRenderMethodSlicerFactory()
  {
    typedef ImageVTKRenderMethodSlicer<ImageType> ImageVTKRenderMethodType;
    this->RegisterOverride("ImageSpatialObject",
                           "ImageVTKRenderMethodSlicer",
                           "ImageSpatialObject as 3 slices Render Method",
                           1,
                           CreateObjectFunction<ImageVTKRenderMethodType>::New());
  }

  template <class ImagePixelType>
  ImageVTKRenderMethodSlicerFactory<ImagePixelType>
  ::~ImageVTKRenderMethodSlicerFactory()
  {
  }

  template <class ImagePixelType>
  const char *
  ImageVTKRenderMethodSlicerFactory<ImagePixelType>
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  template <class ImagePixelType>
  const char *
  ImageVTKRenderMethodSlicerFactory<ImagePixelType>
  ::GetDescription() const
  {
    return "Create ImageVTKRenderMethodSlicer for the visualization of ImageSpatialObject classes";
  }

}
