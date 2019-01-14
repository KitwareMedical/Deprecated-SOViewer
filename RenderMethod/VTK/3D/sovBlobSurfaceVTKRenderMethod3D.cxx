/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobSurfaceVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:56 $
  Version:   $Revision: 1.5 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovBlobSurfaceVTKRenderMethod3D.h"
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkTubeFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>

#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h> 
#include <vtkFloatArray.h>
#include <vtkProperty.h>
#include <vtkContourFilter.h>

#include <itkBlobSpatialObjectToImageFilter.h>
#include "itkImageToVTKImageFilter.h"
#include <itkImage.h>
#include "itkRecursiveGaussianImageFilter.h"
#include <vtkSmoothPolyDataFilter.h>
#include <vtkDecimatePro.h>

namespace sov
{

/** Constructor */
BlobSurfaceVTKRenderMethod3D
::BlobSurfaceVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("BlobSpatialObject");
  m_Smoothness = 0.1; // beetween 0.01 and 0.1(smoother)
  m_Decimation = 0.6; // default = 0.2 (0.1 less decimation 1.0 fully decimated)
  m_Scale = 1;
}

/** Destructor */
BlobSurfaceVTKRenderMethod3D
::~BlobSurfaceVTKRenderMethod3D()
{
}

/** */
void 
BlobSurfaceVTKRenderMethod3D
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
BlobSurfaceVTKRenderMethod3D
::Update()
{
  ObjectListType::iterator            currentObject,lastObject;
 
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
    {
    if((*currentObject)->IsModified())
      {
      RemoveActor(*currentObject); // remove the actor before creating the new one.
      
      std::cout << "CreatingBlob " << std::endl;
      BlobType::Pointer blob = dynamic_cast<BlobType *>((*currentObject)->GetObject());
      typedef itk::Image<double,3> ImageType;   
      typedef itk::BlobSpatialObjectToImageFilter<3,ImageType> CreateImageFilterType;
      
      CreateImageFilterType::Pointer createImageFilter = CreateImageFilterType::New();
      createImageFilter->SetInput(blob);

      createImageFilter->Update();
      ImageType::Pointer image = createImageFilter->GetOutput();

      // std::cout << "Creating VTK Image" << std::endl;
      typedef itk::ImageToVTKImageFilter<ImageType>  ImageToVTKFilterType;
      ImageToVTKFilterType::Pointer imageFilter = ImageToVTKFilterType::New();
      imageFilter->SetInput(image);
      imageFilter->Update();
      vtkImageData* vtkImage = imageFilter->GetOutput();

      vtkImageData* vtkImage2 = vtkImageData::New();
      vtkImage2->DeepCopy(vtkImage);
      //imageFilter->SetReferenceCount(2);

      vtkContourFilter* contourFilter = vtkContourFilter::New();
      contourFilter->SetNumberOfContours(1);
      contourFilter->SetValue(0,0.5);
      //contourFilter->GenerateValues(1,0.8,1.1);
      contourFilter->SetInput(vtkImage2);
      contourFilter->Update();

      vtkCleanPolyData* cleanFilter = vtkCleanPolyData::New();
      cleanFilter->SetInput(contourFilter->GetOutput());
      cleanFilter->Update();

      vtkDecimatePro* decimateFilter = vtkDecimatePro::New();
      decimateFilter->SetInput(cleanFilter->GetOutput());
      decimateFilter->SetTargetReduction(m_Decimation);
      decimateFilter->Update();
        
      std::cout << "Smoothing" << std::endl;
      vtkSmoothPolyDataFilter* smoothFilter = vtkSmoothPolyDataFilter::New();
      smoothFilter->SetInput(decimateFilter->GetOutput());
      smoothFilter->SetNumberOfIterations(100);
      smoothFilter->SetRelaxationFactor(m_Smoothness);
      smoothFilter->Update();

      std::cout << "done !" << std::endl;

      vtkPolyDataMapper* vMapper = vtkPolyDataMapper::New();
      vMapper->SetInput(smoothFilter->GetOutput());
      vMapper->ScalarVisibilityOff();  //do not interpret scalars as color command
   
      vtkActor* actor = vtkActor::New();
      actor->SetMapper(vMapper);

      actor->GetProperty()->SetColor(blob->GetProperty()->GetRed(),
                                     blob->GetProperty()->GetGreen(),
                                     blob->GetProperty()->GetBlue()); 
      actor->GetProperty()->SetOpacity(1.0);

      (*currentObject)->AddVTKDefined(actor);
      (*currentObject)->UpdateMTime();
      m_ActorList.push_back(actor);
      m_ActorToBeAddedList.push_back(actor);
      contourFilter->Delete();
      decimateFilter->Delete();
      cleanFilter->Delete();
      smoothFilter->Delete();
      vMapper->Delete();
      vtkImage2->Delete();
      }

    if((*currentObject)->IsPropertyModified())
      {
      ChangeProperty(* currentObject);
      (*currentObject)->UpdateObjectPropertyMTime();
      }

    if((*currentObject)->IsObjectPropertyModified())
      {
      ChangeProperty(* currentObject);
      (*currentObject)->UpdateObjectPropertyMTime();
      }

    if((*currentObject)->IsTransformModified())
      {
      ApplyTransform(* currentObject);
      (*currentObject)->UpdateTransformMTime();
      }
    } 
}


/** Print the method */
void
BlobSurfaceVTKRenderMethod3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}


} // end of namespace sov
