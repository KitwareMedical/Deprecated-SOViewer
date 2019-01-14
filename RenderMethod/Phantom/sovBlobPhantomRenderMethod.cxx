/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobPhantomRenderMethod.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:55 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovBlobPhantomRenderMethod.h"
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
#include <vtkFloatArray.h>

namespace sov
{

/** Constructor */
BlobPhantomRenderMethod
::BlobPhantomRenderMethod()
{
  m_SupportedTypeList->clear();
  AddSupportedType("BlobSpatialObject");
  m_Smoothness = 0.1; // beetween 0.01 and 0.1(smoother)
  m_Decimation = 0.2; // default = 0.2 (0.1 less decimation 1.0 fully decimated)
  m_Scale = 1;
}

/** Destructor */
BlobPhantomRenderMethod
::~BlobPhantomRenderMethod()
{
}

/** */
void 
BlobPhantomRenderMethod
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
BlobPhantomRenderMethod
::Update()
{
  //std::cout << "BlobPhantomRenderMethod::Update() " << std::endl;
  ObjectListType::iterator            currentObject,lastObject;
 
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  unsigned int tubeIndex=0;
  unsigned int pointIndex=0;

  for(; currentObject!=lastObject; currentObject++)
  {
    if((*currentObject)->IsModified())
    {

      //RemoveActor(*currentObject); // remove the actor before creating the new one.
      
      std::cout << "CreatingBlob " << std::endl;
      BlobType::Pointer blob = dynamic_cast<BlobType *>((*currentObject)->GetObject());
      typedef itk::Image<double,3> ImageType;   
      typedef itk::BlobSpatialObjectToImageFilter<3,ImageType> CreateImageFilterType;
      CreateImageFilterType::Pointer createImageFilter = CreateImageFilterType::New();
      createImageFilter->SetInput(blob);
      //createImageFilter->UseBlobDefaultOrigin(true);
      //createImageFilter->SetSize(size);
      createImageFilter->Update();
      ImageType::Pointer image = createImageFilter->GetOutput();


      typedef itk::ImageToVTKImageFilter<ImageType>  ImageToVTKFilterType;
      ImageToVTKFilterType::Pointer imageFilter = ImageToVTKFilterType::New();
      imageFilter->SetInput(image);
      imageFilter->Update();
      vtkImageData* vtkImage = imageFilter->GetOutput();
      imageFilter->SetReferenceCount(2);

      vtkContourFilter* contourFilter = vtkContourFilter::New();
      contourFilter->SetNumberOfContours(1);
      contourFilter->SetValue(0,0.5);
      //contourFilter->GenerateValues(1,0.8,1.1);
      contourFilter->SetInput(vtkImage);
      contourFilter->Update();

      vtkCleanPolyData* cleanFilter = vtkCleanPolyData::New();
      cleanFilter->SetInput(contourFilter->GetOutput());
      cleanFilter->Update();


      std::cout << "Decimate" << std::endl;
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

      int NumVerts = smoothFilter->GetOutput()->GetNumberOfPoints();
      int NumTris = smoothFilter->GetOutput()->GetNumberOfCells();
     
      
      std::cout << "NumVerts = " << NumVerts << std::endl;
      std::cout << "NumTris = " << NumTris << std::endl;

      // Create the list of vertices
      double* verts;
      verts = new double[NumVerts*3];
      unsigned long j = 0;
      for(unsigned int i=0;i<NumVerts;i++)
        {
        verts[j++] = smoothFilter->GetOutput()->GetPoint(i)[0];
        verts[j++] = smoothFilter->GetOutput()->GetPoint(i)[1];
        verts[j++] = smoothFilter->GetOutput()->GetPoint(i)[2];
        }
        
      int* tris;
      tris = new int[NumTris*3];
      j=0;
      for(unsigned int i=0;i<NumTris;i++)
        {
        tris[j++] = smoothFilter->GetOutput()->GetCell(i)->GetPointId(0);
        tris[j++] = smoothFilter->GetOutput()->GetCell(i)->GetPointId(1);
        tris[j++] = smoothFilter->GetOutput()->GetCell(i)->GetPointId(2);
        }
      
      gwpTriPolyMeshHaptic* triMeshHaptic =  
      new gwpTriPolyMeshHaptic(NumVerts,3,verts, 
                               NumTris,3,tris);
                               
      triMeshHaptic->setTouchableFrom(gwpTriPolyMeshHaptic::RV_FRONT);


      (*currentObject)->AddVTKDefined(triMeshHaptic);
      (*currentObject)->UpdateMTime();
      m_ActorList.push_back(triMeshHaptic);
      m_ActorToBeAddedList.push_back(triMeshHaptic);
      contourFilter->Delete();
      smoothFilter->Delete();
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
BlobPhantomRenderMethod
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}


} // end of namespace sov
