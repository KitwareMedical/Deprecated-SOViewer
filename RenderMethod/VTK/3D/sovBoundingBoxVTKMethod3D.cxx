/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBoundingBoxVTKMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:56 $
  Version:   $Revision: 1.4 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovBoundingBoxVTKRenderMethod3D.h"

#include <itkImageRegionIterator.h>
#include <vtkExtractVOI.h>
#include <vtkPlaneSource.h>
#include <vtkTexture.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOutlineFilter.h>
#include <vtkStructuredPoints.h>
#include <vtkFloatArray.h>
#include <vtkProperty.h>

namespace sov
{

BoundingBoxVTKRenderMethod3D
::BoundingBoxVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("ImageSpatialObject");
  AddSupportedType("TubeSpatialObject");
  AddSupportedType("TubeNetworkSpatialObject");
}

BoundingBoxVTKRenderMethod3D
::~BoundingBoxVTKRenderMethod3D( void )
{
}

void
BoundingBoxVTKRenderMethod3D
::draw( void )
{
  //std::cout << " BoundingBoxVTKRenderMethod3D : draw method not implement yet !" << std::endl;
  // need to implement a base way of rendering BoundingBoxs...
  // let's say wireframe... :)
}

void
BoundingBoxVTKRenderMethod3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

void
BoundingBoxVTKRenderMethod3D
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
      typedef Image<unsigned char,3> ImageType;
      typedef ImageSpatialObject<3,unsigned char> ImageSpatialObjectType;
      ImageType::ConstPointer mImData = dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetImage();

      /*float spacing[3];
      spacing[0] = 1;
      spacing[1] = 1;
      spacing[2] = 1;
      mImData->SetSpacing(spacing);*/

      vtkStructuredPoints* mVol = vtkStructuredPoints::New();
      mVol->SetDimensions(mImData->GetLargestPossibleRegion().GetSize()[0],
                        mImData->GetLargestPossibleRegion().GetSize()[1],
                        mImData->GetLargestPossibleRegion().GetSize()[2]);
    
      mVol->SetSpacing(1.0, 1.0, 1.0);

      vtkFloatArray* mScalars = vtkFloatArray::New();

      vtkActor* mActor = vtkActor::New();
      mActor->SetScale(1, mImData->GetSpacing()[1]/mImData->GetSpacing()[0], mImData->GetSpacing()[2]/mImData->GetSpacing()[0]);
  
      itk::ImageRegionConstIterator<ImageType> it(mImData, mImData->GetLargestPossibleRegion());
      it = it.Begin();
  
      double imMin = 100000 ;
      double imMax = 0;

      long i = 0;
      while (!it.IsAtEnd())
      {
        if (it.Get()<imMin) imMin = it.Get();
        if (it.Get()>imMax) imMax = it.Get();
        ++it;
        i++;
      }
      double imRange = imMax -imMin; 

      mScalars->SetNumberOfTuples(i);
      it = it.Begin();
      i=0 ;
      //for (i = 0; i < (long)mImData->quantity(); i++)
      while (!it.IsAtEnd())
      {
        mScalars->SetTuple1(i, (unsigned char)(((it.Get()-imMin)/imRange)*255));
        i++;
        ++it;
      }  

      //try to put the scalars into the box and render directly
      mVol->GetPointData()->SetScalars(mScalars);

      vtkOutlineFilter * of = vtkOutlineFilter::New();
      of->SetInput(mVol);
      vtkPolyDataMapper * otm = vtkPolyDataMapper::New();
      otm->SetInput(of->GetOutput());
      vtkActor* mOutline = vtkActor::New();
      mOutline->SetMapper(otm);
      mOutline->GetProperty()->SetColor(0, 0, 1);
      double mOpacity = 1;
      mOutline->GetProperty()->SetOpacity(mOpacity);
      mOutline->SetScale(1, mImData->GetSpacing()[1]/mImData->GetSpacing()[0], mImData->GetSpacing()[2]/mImData->GetSpacing()[0]);
    
     (*currentObject)->AddVTKDefined(mOutline);
      m_ActorList.push_back(mOutline);

      of->Delete();
      otm->Delete();
    }

    if((*currentObject)->IsTransformModified())
    {
      ApplyTransform(* currentObject);
      (*currentObject)->UpdateTransformMTime();
    }
  }
}

} // end of namespace sov
