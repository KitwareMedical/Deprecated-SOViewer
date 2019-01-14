/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageVTKRenderMethod3D.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-15 21:36:24 $
  Version:   $Revision: 1.8 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovImageVTKRenderMethod3D.h"

#include <sovVisualImageProperty.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkUnsignedCharArray.h>
#include <vtkShortArray.h>
#include <vtkUnsignedShortArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>

namespace sov
{

template <class TImage>
ImageVTKRenderMethod3D<TImage>
::ImageVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("ImageSpatialObject");
  //m_PixelType = typeid(typename TImage::PixelType).name();
  
  if(typeid(typename TImage::PixelType) == typeid(short))
    {
    m_PixelType = "short";
    }
  else if(typeid(typename TImage::PixelType) == typeid(unsigned char))
    {
    m_PixelType = "unsigned char";
    }
  else if(typeid(typename TImage::PixelType) == typeid(unsigned short))
    {
    m_PixelType = "unsigned short";
    }
  else if(typeid(typename TImage::PixelType) == typeid(float))
    {
    m_PixelType = "float";
    }
  else if(typeid(typename TImage::PixelType) == typeid(double))
    {
    m_PixelType = "double";
    }
}

template <class TImage>
ImageVTKRenderMethod3D<TImage>
::~ImageVTKRenderMethod3D( void )
{
  m_PropIt = m_ImagePropList.begin();
  while(m_PropIt != m_ImagePropList.end())
  {
    ImageProp* prop = *m_PropIt;
    m_PropIt++;
    delete prop;
  }
}

template <class TImage>
void
ImageVTKRenderMethod3D<TImage>
::Draw( void )
{
  if(static_cast<VisualImageProperty*>((*m_CurrentObject)->GetProperty())->GetUseIntensityRange())
    {
    double min = static_cast<VisualImageProperty*>((*m_CurrentObject)->GetProperty())->GetIntensityRange()[0];
    double max = static_cast<VisualImageProperty*>((*m_CurrentObject)->GetProperty())->GetIntensityRange()[1];
    double level = min+(max-min)/2;
    double window = max-min;

    float minialpha = static_cast<VisualImageProperty*>((*m_CurrentObject)->GetProperty())->GetMinimalAlphaValue();
    (*m_PropIt)->LookupTable->SetLevel(level);
    (*m_PropIt)->LookupTable->SetWindow(window);
    (*m_PropIt)->LookupTable->Build();
    if(minialpha>=0 && minialpha <=1)
      {
      (*m_PropIt)->LookupTable->SetTableValue(0,0,0,0,minialpha);
      }
    (*m_PropIt)->SaggitalText->MapColorScalarsThroughLookupTableOn();
    (*m_PropIt)->AxialText->MapColorScalarsThroughLookupTableOn();
    (*m_PropIt)->CoronalText->MapColorScalarsThroughLookupTableOn();
    }
  else
    {
    (*m_PropIt)->SaggitalText->MapColorScalarsThroughLookupTableOff();
    (*m_PropIt)->AxialText->MapColorScalarsThroughLookupTableOff();
    (*m_PropIt)->CoronalText->MapColorScalarsThroughLookupTableOff();
    }

  if(m_ImData->GetLargestPossibleRegion().GetSize()[0]>1 && m_ImData->GetLargestPossibleRegion().GetSize()[2]>1)
    {
    (*m_PropIt)->Saggital->GetProperty()->SetOpacity(static_cast<VisualImageProperty*>((*m_CurrentObject)->GetProperty())->GetOpacity(1));

    (*m_PropIt)->SaggitalSlice = dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetSlicePosition(1);    
    (*m_PropIt)->SaggitalSect->SetVOI(0, m_ImData->GetLargestPossibleRegion().GetSize()[0]-1, 
                              (*m_PropIt)->SaggitalSlice,(*m_PropIt)->SaggitalSlice, 0, m_ImData->GetLargestPossibleRegion().GetSize()[2]-1);
    (*m_PropIt)->SaggitalSect->SetInput((*m_PropIt)->Vol);
    (*m_PropIt)->SaggitalText->SetInput((*m_PropIt)->SaggitalSect->GetOutput());
    (*m_PropIt)->SaggitalPlane->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1]+(*m_PropIt)->SaggitalSlice,m_ImData->GetOrigin()[2]);
    (*m_PropIt)->SaggitalPlane->SetPoint1(m_ImData->GetOrigin()[0]+m_ImData->GetLargestPossibleRegion().GetSize()[0]-1,m_ImData->GetOrigin()[1]+(*m_PropIt)->SaggitalSlice, m_ImData->GetOrigin()[2]);
    (*m_PropIt)->SaggitalPlane->SetPoint2(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1]+(*m_PropIt)->SaggitalSlice, m_ImData->GetOrigin()[2]+m_ImData->GetLargestPossibleRegion().GetSize()[2]-1);
    (*m_PropIt)->Saggital->SetScale(m_ImData->GetSpacing()[0], m_ImData->GetSpacing()[1], m_ImData->GetSpacing()[2]);
    //(*m_PropIt)->Saggital->GetProperty()->SetOpacity(0.5);
    (*m_PropIt)->Saggital->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1], m_ImData->GetOrigin()[2]);

    }

  if(m_ImData->GetLargestPossibleRegion().GetSize()[0]>1 && m_ImData->GetLargestPossibleRegion().GetSize()[1]>1)
    {
    (*m_PropIt)->Axial->GetProperty()->SetOpacity(static_cast<VisualImageProperty *>((*m_CurrentObject)->GetProperty())->GetOpacity(2));
    (*m_PropIt)->AxialSlice = dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetSlicePosition(2);
    (*m_PropIt)->AxialSect->SetVOI(0, m_ImData->GetLargestPossibleRegion().GetSize()[0]-1,  0, m_ImData->GetLargestPossibleRegion().GetSize()[1]-1,
    (*m_PropIt)->AxialSlice, (*m_PropIt)->AxialSlice);
    (*m_PropIt)->AxialSect->SetInput((*m_PropIt)->Vol);
    (*m_PropIt)->AxialText->SetInput((*m_PropIt)->AxialSect->GetOutput());
    //(*m_PropIt)->AxialPlane->SetOrigin(0, 0, (*m_PropIt)->AxialSlice);
    (*m_PropIt)->AxialPlane->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1],(*m_PropIt)->AxialSlice+m_ImData->GetOrigin()[2]);
    (*m_PropIt)->AxialPlane->SetPoint1(m_ImData->GetOrigin()[0]+m_ImData->GetLargestPossibleRegion().GetSize()[0]-1, m_ImData->GetOrigin()[1],m_ImData->GetOrigin()[2]+(*m_PropIt)->AxialSlice);
    (*m_PropIt)->AxialPlane->SetPoint2(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1]+m_ImData->GetLargestPossibleRegion().GetSize()[1]-1, m_ImData->GetOrigin()[2]+(*m_PropIt)->AxialSlice);
    (*m_PropIt)->Axial->SetScale(m_ImData->GetSpacing()[0], m_ImData->GetSpacing()[1], m_ImData->GetSpacing()[2]);   
    (*m_PropIt)->Axial->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1], m_ImData->GetOrigin()[2]);
    }

  if(m_ImData->GetLargestPossibleRegion().GetSize()[2]>1 && m_ImData->GetLargestPossibleRegion().GetSize()[1]>1)
    {
    (*m_PropIt)->Coronal->GetProperty()->SetOpacity(static_cast<VisualImageProperty *>((*m_CurrentObject)->GetProperty())->GetOpacity(0));
    (*m_PropIt)->CoronalSlice = dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetSlicePosition(0);
    (*m_PropIt)->CoronalSect->SetVOI((*m_PropIt)->CoronalSlice, (*m_PropIt)->CoronalSlice, 0, m_ImData->GetLargestPossibleRegion().GetSize()[1]-1, 0, m_ImData->GetLargestPossibleRegion().GetSize()[2]-1);
    (*m_PropIt)->CoronalSect->SetInput((*m_PropIt)->Vol);
    (*m_PropIt)->CoronalText->SetInput((*m_PropIt)->CoronalSect->GetOutput());
    //(*m_PropIt)->CoronalPlane->SetOrigin((*m_PropIt)->CoronalSlice, 0, 0);
    (*m_PropIt)->CoronalPlane->SetOrigin((*m_PropIt)->CoronalSlice+m_ImData->GetOrigin()[0],m_ImData->GetOrigin()[1],m_ImData->GetOrigin()[2]);
    (*m_PropIt)->CoronalPlane->SetPoint1(m_ImData->GetOrigin()[0]+(*m_PropIt)->CoronalSlice,m_ImData->GetOrigin()[1]+ m_ImData->GetLargestPossibleRegion().GetSize()[1]-1, m_ImData->GetOrigin()[2]);
    (*m_PropIt)->CoronalPlane->SetPoint2(m_ImData->GetOrigin()[0]+(*m_PropIt)->CoronalSlice, m_ImData->GetOrigin()[1], m_ImData->GetOrigin()[2]+m_ImData->GetLargestPossibleRegion().GetSize()[2]-1);
    (*m_PropIt)->Coronal->SetScale(m_ImData->GetSpacing()[0], m_ImData->GetSpacing()[1], m_ImData->GetSpacing()[2]);
    (*m_PropIt)->Coronal->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1], m_ImData->GetOrigin()[2]);
 
    }  
  (*m_CurrentObject)->UpdateMTime();
}

template <class TImage>
void
ImageVTKRenderMethod3D<TImage>
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

  
template <class TImage>
void
ImageVTKRenderMethod3D<TImage>
::Update()
{ 
  m_PropIt = m_ImagePropList.begin();
  // Check if m_ImagePropList is up to date (image removed?)
  while(m_PropIt != m_ImagePropList.end())
    {
    bool found = false;
    for(m_CurrentObject = m_ObjectList.begin(); m_CurrentObject!=m_ObjectList.end(); m_CurrentObject++)
      {
      if((*m_PropIt)->Object == (*m_CurrentObject)->GetObject())
        {
        found = true;
        }
      }

    if(!found)
      {
      m_PropIt = m_ImagePropList.erase(m_PropIt);
      }

    if(m_PropIt != m_ImagePropList.end())
      {
      m_PropIt++;
      }
    }

  m_PropIt = m_ImagePropList.begin();
  unsigned int id = 0;

  m_CurrentObject = m_ObjectList.begin();
  for(; m_CurrentObject!=m_ObjectList.end(); m_CurrentObject++)
    {
    bool first = false;
    m_ImData = dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetImage();
   
    /** First Time we add the object properties to the list */
    if(id>=m_ImagePropList.size())
      {
      ImageProp* prop = new ImageProp;
      prop->ID=id;
      prop->Image = m_ImData.GetPointer();
      prop->Object = (*m_CurrentObject)->GetObject();
      prop->MTime = m_ImData->GetMTime();
      m_ImagePropList.push_back(prop);
      m_PropIt = std::find(m_ImagePropList.begin(),m_ImagePropList.end(),prop);
      first = true;
      }

    if((*m_CurrentObject)->IsModified())
      {
      bool shouldDraw = false;
      if(!first)
        {
        //GetPipelineMTime
        if((*m_PropIt)->MTime >= m_ImData->GetMTime()) // if the internal image is not modified we just redraw
          {
          (*m_PropIt)->MTime = m_ImData->GetMTime();
          this->Draw();
          shouldDraw = true;
          }
        else
          {    
          (*m_PropIt)->MTime = m_ImData->GetMTime();  
          
          // if the object is different we clean and renew
          if(first)
            {
            RemoveActor(*m_CurrentObject); // remove the actor before creating the new one.
          
            (*m_CurrentObject)->RemoveVTKDefined((*m_PropIt)->Axial);
            (*m_CurrentObject)->RemoveVTKDefined((*m_PropIt)->Coronal);
            (*m_CurrentObject)->RemoveVTKDefined((*m_PropIt)->Saggital);

            delete (*m_PropIt);
            (*m_PropIt) = new ImageProp;
            }
          
          }
        }
       
      if(!shouldDraw)
      {   
        unsigned long dim = m_ImData->GetLargestPossibleRegion().GetSize()[0]*m_ImData->GetLargestPossibleRegion().GetSize()[1]*m_ImData->GetLargestPossibleRegion().GetSize()[2];

        if(first)
          {
          (*m_PropIt)->Vol->SetDimensions(m_ImData->GetLargestPossibleRegion().GetSize()[0],
                            m_ImData->GetLargestPossibleRegion().GetSize()[1],
                            m_ImData->GetLargestPossibleRegion().GetSize()[2]);
    
          (*m_PropIt)->Vol->SetSpacing(m_ImData->GetSpacing()[0],m_ImData->GetSpacing()[1],m_ImData->GetSpacing()[2]);

          vtkDataArray* mScalars;
          if(!strcmp(dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetPixelType(),"short"))
            {
            mScalars = vtkShortArray::New(); 
            }
          else if(!strcmp(dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetPixelType(),"unsigned char"))
            {
            mScalars = vtkUnsignedCharArray::New(); 
            }
          else if(!strcmp(dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetPixelType(),"unsigned short"))
            {
            mScalars = vtkUnsignedShortArray::New(); 
            }
          else if(!strcmp(dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetPixelType(),"float"))
            {
            mScalars = vtkFloatArray::New(); 
            }
          else if(!strcmp(dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetPixelType(),"double"))
            {
            mScalars = vtkDoubleArray::New(); 
            }
          else
            {
            std::cout << "ImageVTKRenderMethod3D cannot create vtkDataArray from this pixel type : ";
            std::cout << dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetPixelType() << std::endl;
            return;
            }

          mScalars->SetNumberOfTuples(dim);
          (*m_PropIt)->Vol->GetPointData()->SetScalars(mScalars); 
          mScalars->Delete();
          }

        (*m_PropIt)->Vol->GetPointData()->GetScalars()->SetVoidArray((void*)(m_ImData->GetBufferPointer()),dim,1);
        (*m_PropIt)->Vol->Modified();       
        /*itk::ImageRegionConstIterator<ImageType> it(m_ImData, m_ImData->GetLargestPossibleRegion());
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
 
        while (!it.IsAtEnd())
        {
          mScalars->SetTuple1(i, (unsigned char)(((it.Get()-imMin)/imRange)*255));
          i++;
          ++it;
        }*/ 
      
        //try to put the scalars into the box and render directly
        //(*m_PropIt)->Vol->GetPointData()->SetScalars2(mScalars);   
             
         if(first)
          {
          (*m_PropIt)->SaggitalMap->SetColorModeToMapScalars();
          (*m_PropIt)->SaggitalText->InterpolateOn();
          (*m_PropIt)->SaggitalPlane->SetXResolution(2);
          (*m_PropIt)->SaggitalPlane->SetYResolution(2);
          (*m_PropIt)->SaggitalMap->SetInput((*m_PropIt)->SaggitalPlane->GetOutput());
          (*m_PropIt)->SaggitalMap->ImmediateModeRenderingOn();
          (*m_PropIt)->Saggital->SetMapper((*m_PropIt)->SaggitalMap);
          (*m_PropIt)->Saggital->SetTexture((*m_PropIt)->SaggitalText);
          (*m_PropIt)->Saggital->GetProperty()->SetOpacity(static_cast<VisualImageProperty*>((*m_CurrentObject)->GetProperty())->GetOpacity(0));
          (*m_PropIt)->Saggital->GetProperty()->SetInterpolationToFlat();
          (*m_PropIt)->Saggital->SetScale(m_ImData->GetSpacing()[0], m_ImData->GetSpacing()[1], m_ImData->GetSpacing()[2]);
          (*m_PropIt)->Saggital->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1], m_ImData->GetOrigin()[2]);
       
          (*m_CurrentObject)->AddVTKDefined((*m_PropIt)->Saggital);
          m_ActorList.push_back((*m_PropIt)->Saggital);
          m_ActorToBeAddedList.push_back((*m_PropIt)->Saggital);

          (*m_PropIt)->AxialPlane->SetXResolution(2);
          (*m_PropIt)->AxialPlane->SetYResolution(2);
          (*m_PropIt)->AxialText->InterpolateOn();
          (*m_PropIt)->AxialMap->SetColorModeToMapScalars();
          (*m_PropIt)->AxialMap->SetInput((*m_PropIt)->AxialPlane->GetOutput());
          (*m_PropIt)->AxialMap->ImmediateModeRenderingOn();
          (*m_PropIt)->Axial->SetMapper((*m_PropIt)->AxialMap);
          (*m_PropIt)->Axial->SetTexture((*m_PropIt)->AxialText);
          (*m_PropIt)->Axial->GetProperty()->SetOpacity(static_cast<VisualImageProperty *>((*m_CurrentObject)->GetProperty())->GetOpacity(1));
          (*m_PropIt)->Axial->GetProperty()->SetInterpolationToFlat();
          (*m_PropIt)->Axial->SetScale(m_ImData->GetSpacing()[0], m_ImData->GetSpacing()[1], m_ImData->GetSpacing()[2]);
          (*m_PropIt)->Axial->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1], m_ImData->GetOrigin()[2]);
 
          (*m_CurrentObject)->AddVTKDefined((*m_PropIt)->Axial); 
          m_ActorList.push_back((*m_PropIt)->Axial);
          m_ActorToBeAddedList.push_back((*m_PropIt)->Axial);
 
        
          (*m_PropIt)->CoronalMap->SetColorModeToMapScalars();
          (*m_PropIt)->CoronalText->InterpolateOn();
          (*m_PropIt)->CoronalPlane->SetXResolution(2);
          (*m_PropIt)->CoronalPlane->SetYResolution(2);
          (*m_PropIt)->CoronalMap->SetInput((*m_PropIt)->CoronalPlane->GetOutput());
          (*m_PropIt)->CoronalMap->ImmediateModeRenderingOn();
          (*m_PropIt)->Coronal->SetMapper((*m_PropIt)->CoronalMap);
          (*m_PropIt)->Coronal->SetTexture((*m_PropIt)->CoronalText);
          (*m_PropIt)->Coronal->GetProperty()->SetOpacity(static_cast<VisualImageProperty *>((*m_CurrentObject)->GetProperty())->GetOpacity(2));
          (*m_PropIt)->Coronal->GetProperty()->SetInterpolationToFlat();
          (*m_PropIt)->Coronal->SetScale(m_ImData->GetSpacing()[0], m_ImData->GetSpacing()[1], m_ImData->GetSpacing()[2]);
          (*m_PropIt)->Coronal->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1], m_ImData->GetOrigin()[2]);

     
          (*m_CurrentObject)->AddVTKDefined((*m_PropIt)->Coronal);
          m_ActorList.push_back((*m_PropIt)->Coronal);
          m_ActorToBeAddedList.push_back((*m_PropIt)->Coronal);
           }
     
        //mScalars->Delete();
        this->Draw();
        ApplyTransform(* m_CurrentObject);
        (*m_CurrentObject)->UpdateTransformMTime();
      }
    }
  
    if((*m_CurrentObject)->IsPropertyModified())
    { 
      ChangeProperty(* m_CurrentObject);
      (*m_CurrentObject)->UpdateObjectPropertyMTime();
    }


    if((*m_CurrentObject)->IsObjectPropertyModified())
    { 
      ChangeProperty(* m_CurrentObject);
      (*m_CurrentObject)->UpdateObjectPropertyMTime();
    }

    if((*m_CurrentObject)->IsTransformModified())
    {
      ApplyTransform(* m_CurrentObject);
      (*m_CurrentObject)->UpdateTransformMTime();
    }

    id++;
    m_PropIt++;
  }

}

} // end of namespace sov
