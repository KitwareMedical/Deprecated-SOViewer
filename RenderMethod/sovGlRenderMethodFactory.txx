/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderMethodFactory.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-09 18:34:48 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <itkCreateObjectFunction.h>
#include <itkVersion.h>

#include <cstring>

#include "sovGlRenderMethodFactory.h"
#include "sovRenderMethod.h"
#include <itkRGBPixel.h>

// Factories to be registered
// OpenGL Slicer
#include <sovImageGlRenderMethodSlicerFactory.h>
#include <sovSurfaceTubeGlRenderMethodSlicerFactory.h>
#include <sovLandmarkGlRenderMethodSlicerFactory.h>

// OpenGL 2D
#include <sovImageGlRenderMethod2DFactory.h>
#include <sovColorImageGlRenderMethod2DFactory.h>
#include <sovEllipseGlRenderMethod2DFactory.h>
#include <sovBlobGlRenderMethod2DFactory.h>

namespace sov
{


/** Returns a pointer to the method specified as argument.
 *  In that case, it just check if one of the instances returned
 *  by the object factories contains the requested name in its
 *  name */
template < unsigned int TDimension >
typename GlRenderMethodFactory<TDimension>::RenderMethodPointer
GlRenderMethodFactory<TDimension>::CreateRenderMethod( const char * method, unsigned int itkNotUsed(dimension), 
                                                     const char* type, const char* methodName, const char* pixelType)
  {   

    ViewerFactoryBase::UnRegisterAllFactories();
  
// Register the factories
if(TDimension == 3)
  {
    /**** Slicer OpenGL Factories  ****/
    ViewerFactoryBase::RegisterFactory(ImageGlRenderMethodSlicerFactory<unsigned char>::New());
    ViewerFactoryBase::RegisterFactory(ImageGlRenderMethodSlicerFactory<unsigned short>::New());
    ViewerFactoryBase::RegisterFactory(ImageGlRenderMethodSlicerFactory<float>::New());
    ViewerFactoryBase::RegisterFactory(ImageGlRenderMethodSlicerFactory<double>::New());
    ViewerFactoryBase::RegisterFactory(ImageGlRenderMethodSlicerFactory<short>::New());
    ViewerFactoryBase::RegisterFactory(SurfaceTubeGlRenderMethodSlicerFactory::New());
    ViewerFactoryBase::RegisterFactory(LandmarkGlRenderMethodSlicerFactory::New());
  }
else if(TDimension == 2)
  {
  /**** 2D OpenGL Factories  ****/
  ViewerFactoryBase::RegisterFactory(ImageGlRenderMethod2DFactory<unsigned char>::New());
  ViewerFactoryBase::RegisterFactory(ImageGlRenderMethod2DFactory<unsigned short>::New());
  ViewerFactoryBase::RegisterFactory(ImageGlRenderMethod2DFactory<float>::New());
  ViewerFactoryBase::RegisterFactory(ImageGlRenderMethod2DFactory<double>::New());
  ViewerFactoryBase::RegisterFactory(ImageGlRenderMethod2DFactory<short>::New());

  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  ViewerFactoryBase::RegisterFactory(ColorImageGlRenderMethod2DFactory<RGBPixelType>::New());
  
  ViewerFactoryBase::RegisterFactory(EllipseGlRenderMethod2DFactory::New());
  ViewerFactoryBase::RegisterFactory(BlobGlRenderMethod2DFactory::New());

  }

  typedef typename RenderMethodType::Pointer  RenderMethodPointer;
  std::list< RenderMethodPointer >            renderMethodList;
  std::list< LightObject::Pointer >           objectList;
  std::list< RenderMethodPointer >            methodList;
  objectList = ViewerFactoryBase::CreateAllInstance(method);


  for(std::list<LightObject::Pointer>::iterator i = objectList.begin();
      i != objectList.end(); ++i)
    {
    RenderMethodType* meth = dynamic_cast<RenderMethodType*>(i->GetPointer());
    if(meth)
      {
      if(!methodName)
      {
        methodList.push_back(meth);
      }
      else if(!strcmp((*i)->GetNameOfClass(),methodName))
      {
        methodList.push_back(meth);
      }
      }
    else
      {
      std::cerr << "Error OpenGLRenderMethod factory did not return a valid RenderMethod type : "
                << (*i)->GetNameOfClass() 
                << std::endl;
      }
    }

  // return the first instance able to render the object
  for(typename std::list<RenderMethodPointer>::iterator k = methodList.begin();
      k != methodList.end(); ++k)
    { 
      if(!strcmp(type,(*k)->GetType()))
      {
      if((!strcmp(method,"ImageSpatialObject")) || (!strcmp(method,"MeshSpatialObject"))) // if the method is for an image we check the pixel type
      {
       if(!strcmp((*k)->GetPixelType(),pixelType))
        {
          return *k;
        }
      }
      else
      {   
        return *k;
      }
      }
    }
  return 0;  
}

template < unsigned int TDimension >
void 
GlRenderMethodFactory<TDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  // nothing to print out at that time...
}

} //end of namespace sov
