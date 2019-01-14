/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRenderMethodFactory.txx,v $
  Language:  C++
  Date:      $Date: 2005-12-15 19:45:40 $
  Version:   $Revision: 1.9 $
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

#include "sovVTKRenderMethodFactory.h"
#include "sovRenderMethod.h"
#include <itkRGBPixel.h>

// Factories to be registered
// VTK
#include <sovSurfaceTubeVTKRenderMethod3DFactory.h>
#include <sovSphereVTKRenderMethod3DFactory.h>
#include <sovArrowVTKRenderMethod3DFactory.h>
#include <sovCenterLineTubeVTKRenderMethod3DFactory.h>
#include <sovImageVTKRenderMethod3DFactory.h>
#include <sovWireframeMeshVTKRenderMethod3DFactory.h>
#include <sovSurfaceMeshVTKRenderMethod3DFactory.h>
#include <sovBlobPointVTKRenderMethod3DFactory.h>
#include <sovBlobSurfaceVTKRenderMethod3DFactory.h>
#include <sovBoxVTKRenderMethod3DFactory.h>
#include <sovLandmarkVTKRenderMethod3DFactory.h>
#include <sovSurfacePointVTKRenderMethod3DFactory.h>
#include <sovSparsePointVTKRenderMethod3DFactory.h>
#include <sovDTITubeTensorVTKRenderMethod3DFactory.h>
#include <sovDeformableVesselTubeVTKRenderMethod3DFactory.h>
#include <sovSurfaceNormalVectorVTKRenderMethod3DFactory.h>
#include <sovCylinderVTKRenderMethod3DFactory.h>
#include <sovAttributeMeshVTKRenderMethod3DFactory.h>

// Slicer Factories
#include <sovImageVTKRenderMethodSlicerFactory.h>
#include <sovSurfaceTubeVTKRenderMethodSlicerFactory.h>
#include <sovSphereVTKRenderMethodSlicerFactory.h>

namespace sov
{


/** Returns a pointer to the method specified as argument.
 *  In that case, it just check if one of the instances returned
 *  by the object factories contains the requested name in its
 *  name */
template < unsigned int TDimension >
typename VTKRenderMethodFactory<TDimension>::RenderMethodPointer
VTKRenderMethodFactory<TDimension>::CreateRenderMethod( const char * method, unsigned int itkNotUsed(dimension), 
                                                     const char* type, const char* methodName, const char* pixelType)
  {   
     
  ViewerFactoryBase::UnRegisterAllFactories();
// Register the factories
if(!strcmp(type,"VTKSlicer"))
  {
  ViewerFactoryBase::RegisterFactory(SphereVTKRenderMethodSlicerFactory::New() ); 
  ViewerFactoryBase::RegisterFactory(SurfaceTubeVTKRenderMethodSlicerFactory::New() ); 
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethodSlicerFactory<unsigned char>::New());
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethodSlicerFactory<unsigned short>::New());
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethodSlicerFactory<float>::New());
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethodSlicerFactory<double>::New());
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethodSlicerFactory<short>::New());
  }
else if(!strcmp(type,"VTK3D"))
  {
  /**** 3D VTK Factories  ****/
  ViewerFactoryBase::RegisterFactory( BoxVTKRenderMethod3DFactory::New() ); 
  ViewerFactoryBase::RegisterFactory( SurfaceTubeVTKRenderMethod3DFactory::New() ); 
  ViewerFactoryBase::RegisterFactory( SphereVTKRenderMethod3DFactory::New() ); 
  ViewerFactoryBase::RegisterFactory( ArrowVTKRenderMethod3DFactory::New() ); 
  ViewerFactoryBase::RegisterFactory( DTITubeTensorVTKRenderMethod3DFactory::New());
  ViewerFactoryBase::RegisterFactory( CenterLineTubeVTKRenderMethod3DFactory::New() ); 
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethod3DFactory<unsigned char>::New());
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethod3DFactory<unsigned short>::New());
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethod3DFactory<float>::New());
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethod3DFactory<double>::New());
  ViewerFactoryBase::RegisterFactory(ImageVTKRenderMethod3DFactory<short>::New());
  ViewerFactoryBase::RegisterFactory(BlobSurfaceVTKRenderMethod3DFactory::New());
  ViewerFactoryBase::RegisterFactory(BlobPointVTKRenderMethod3DFactory::New());
  ViewerFactoryBase::RegisterFactory(LandmarkVTKRenderMethod3DFactory::New());
  ViewerFactoryBase::RegisterFactory(SurfacePointVTKRenderMethod3DFactory::New());
  ViewerFactoryBase::RegisterFactory(SparsePointVTKRenderMethod3DFactory::New());
  ViewerFactoryBase::RegisterFactory(DeformableVesselTubeVTKRenderMethod3DFactory::New());
  ViewerFactoryBase::RegisterFactory(SurfaceNormalVectorVTKRenderMethod3DFactory::New());
  ViewerFactoryBase::RegisterFactory(CylinderVTKRenderMethod3DFactory::New());

  // Mesh RenderMethods
  typedef itk::DefaultDynamicMeshTraits< float , 3, 3 > MeshTrait;
  typedef itk::Mesh<float,3,MeshTrait>  MeshType;
  ViewerFactoryBase::RegisterFactory( SurfaceMeshVTKRenderMethod3DFactory<MeshType>::New());
  ViewerFactoryBase::RegisterFactory( WireframeMeshVTKRenderMethod3DFactory<MeshType>::New());

  typedef itk::VariableLengthVector<float> AttributeMeshVectorType;
  //typedef itk::Vector<float,3> AttributeMeshVectorType;
  typedef itk::DefaultDynamicMeshTraits<AttributeMeshVectorType, 3, 3> AttributeMeshTrait;
  typedef itk::Mesh<float,3,AttributeMeshTrait>  AttributeMeshType;
  ViewerFactoryBase::RegisterFactory( AttributeMeshVTKRenderMethod3DFactory<AttributeMeshType>::New());
  }
else if(TDimension == 2)
  {
  }






  typedef typename RenderMethodType::Pointer  RenderMethodPointer;
  std::list< RenderMethodPointer >            renderMethodList;
  typename std::list< RenderMethodPointer >::iterator  renderMethodIterator;
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
      std::cerr << "Error VTKRenderMethod factory did not return a valid RenderMethod type : "
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
VTKRenderMethodFactory<TDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  // nothing to print out at that time...
}

} //end of namespace sov
