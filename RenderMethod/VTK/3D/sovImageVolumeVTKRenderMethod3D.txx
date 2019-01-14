/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageVolumeVTKRenderMethod3D.txx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:57 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovImageVolumeVTKRenderMethod3D.h"

#include <sovVisualImageProperty.h>

#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>

namespace sov
{

template <class TImage>
ImageVolumeVTKRenderMethod3D<TImage>
::ImageVolumeVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("ImageSpatialObject");
}

template <class TImage>
ImageVolumeVTKRenderMethod3D<TImage>
::~ImageVolumeVTKRenderMethod3D( void )
{
}

template <class TImage>
void
ImageVolumeVTKRenderMethod3D<TImage>
::draw( void )
{
  //std::cout << "ImageVolumeVTKRenderMethod3D::Draw()" << std::endl;

  ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
   
  for(; currentObject!=lastObject; currentObject++)
  {
    typename ImageType::ConstPointer mImData = dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetImage();
  }
}

template <class TImage>
void
ImageVolumeVTKRenderMethod3D<TImage>
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

  
template <class TImage>
void
ImageVolumeVTKRenderMethod3D<TImage>
::Update()
{

  //std::cout << "ImageVolumeVTKRenderMethod3D::Update()" << std::endl;
  
  ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
   
  for(; currentObject!=lastObject; currentObject++)
  {
    if((*currentObject)->IsModified())
    {
      RemoveActor(*currentObject); // remove the actor before creating the new one.
      typename ImageType::ConstPointer mImData = dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetImage();

      std::cout << "Creating VTK Image" << std::endl;
      typedef itk::ImageToVTKImageFilter<ImageType>  ImageToVTKFilterType;
      typename ImageToVTKFilterType::Pointer imageFilter 
                                            = ImageToVTKFilterType::New();
      imageFilter->SetInput(mImData);
      imageFilter->Update();
      vtkImageData* vtkImage = imageFilter->GetOutput();
      imageFilter->SetReferenceCount(2);
    

      vtkPiecewiseFunction *oTFun = vtkPiecewiseFunction::New();
      oTFun->AddSegment(150,0.0,255,1.0);

      vtkPiecewiseFunction *gTFun = vtkPiecewiseFunction::New();
      gTFun->AddSegment(0,1.0,255,1.0);

      vtkVolumeProperty *volProperty = vtkVolumeProperty::New();
      volProperty->SetColor(gTFun);
      volProperty->SetScalarOpacity(oTFun);
      volProperty->SetInterpolationTypeToLinear();
      volProperty->ShadeOn();

      vtkVolumeRayCastCompositeFunction *compositeFunction =
        vtkVolumeRayCastCompositeFunction::New();
     
      vtkVolumeRayCastMapper * volMapper = vtkVolumeRayCastMapper::New();
      volMapper->SetInput(vtkImage);
      volMapper->SetVolumeRayCastFunction(compositeFunction);

      vtkVolume* actor = vtkVolume::New();
      actor->SetScale(mImData->GetSpacing()[0], mImData->GetSpacing()[1], mImData->GetSpacing()[2]);
      actor->SetMapper(volMapper);
      actor->SetProperty(volProperty);
      (*currentObject)->AddVTKDefined(actor);
      m_ActorList.push_back(actor);
      m_ActorToBeAddedList.push_back(actor);
      (*currentObject)->UpdateMTime();
    }

    if((*currentObject)->IsTransformModified())
    {
      ApplyTransform(* currentObject);
      (*currentObject)->UpdateTransformMTime();
    }

  }
  this->draw();
}

} // end of namespace sov
