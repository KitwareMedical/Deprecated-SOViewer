/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageVTKRenderMethodSlicer.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-09 15:37:03 $
  Version:   $Revision: 1.5 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovImageVTKRenderMethodSlicer.h"

#include <sovVisualImageProperty.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkUnsignedCharArray.h>

namespace sov
{

template <class TImage>
ImageVTKRenderMethodSlicer<TImage>
::ImageVTKRenderMethodSlicer( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("ImageSpatialObject");
  m_PixelType = typeid(typename TImage::PixelType).name();
}

template <class TImage>
ImageVTKRenderMethodSlicer<TImage>
::~ImageVTKRenderMethodSlicer( void )
{
  m_PropIt = m_ImagePropList.begin();
  while(m_PropIt != m_ImagePropList.end())
    {
    ImagePropSlicer* prop = *m_PropIt;
    m_PropIt++;
    delete prop;
    }
}

template <class TImage>
void
ImageVTKRenderMethodSlicer<TImage>
::Draw( void )
{
  if(static_cast<VisualImageProperty*>((*m_CurrentObject)->GetProperty())->GetUseIntensityRange())
    {
    double min = static_cast<VisualImageProperty*>((*m_CurrentObject)->GetProperty())->GetIntensityRange()[0];
    double max = static_cast<VisualImageProperty*>((*m_CurrentObject)->GetProperty())->GetIntensityRange()[1];
    double level = min+(max-min)/2;
    double window = max-min;

    (*m_PropIt)->LookupTable->SetLevel(level);
    (*m_PropIt)->LookupTable->SetWindow(window);
    (*m_PropIt)->LookupTable->Build();
    (*m_PropIt)->Texture->MapColorScalarsThroughLookupTableOn();
    }
  else
    {
    //(*m_PropIt)->SaggitalText->MapColorScalarsThroughLookupTableOff();
    (*m_PropIt)->Texture->MapColorScalarsThroughLookupTableOff();
    //(*m_PropIt)->CoronalText->MapColorScalarsThroughLookupTableOff();
    }

  //m_AxialSlice = m_Image->GetLargestPossibleRegion().GetSize()[1]-m_SlicePosition[1]-1;    
  //m_AxialSlice = m_SlicePosition[1];    
 /*   
  itk::Versor<float> v1;
  itk::Vector<float,3> axis1;
     
  axis1.Fill(0);
  axis1[0]=1;     
  v1.Set(axis1,static_cast<b3dGUIControls*>(m_GUI)->GetAxialAngle(0));

  itk::Versor<float> v2;
  itk::Vector<float,3> axis2;
  axis2.Fill(0);
  axis2[2]=1;     
  v2.Set(axis2,static_cast<b3dGUIControls*>(m_GUI)->GetAxialAngle(1));

  itk::ScalableAffineTransform<float>::Pointer affine2 = itk::ScalableAffineTransform<float>::New();
      
  itk::Point<float,3> cor2;
  cor2[0] = m_SlicePosition[0]*spacing[0];
  cor2[1] = ((double)(m_Image->GetLargestPossibleRegion().GetSize()[1])/2.0)*spacing[1];
  cor2[2] = m_SlicePosition[2]*spacing[2];
  affine2->SetMatrix(v1.GetMatrix()*v2.GetMatrix());
  affine2->SetCenterOfRotationComponent(cor2);
     
  itk::Vector<float,3> offset;
  offset[0]=0;
  offset[1]=((m_AxialSlice)-(double)m_Image->GetLargestPossibleRegion().GetSize()[1]/2)*spacing[1];
  offset[2]=0;

  affine2->SetOffsetComponent(offset);
*/

  //itk::ScalableAffineTransform<float>::Pointer affine2 = itk::ScalableAffineTransform<float>::New();

//  itk::Matrix<float,3,3> rotationMatrix = v1.GetMatrix()*v2.GetMatrix();
  vtkMatrix4x4* vtkMatrix = vtkMatrix4x4::New();

  for(unsigned int i=0;i<3;i++)
    {
      for(unsigned int j=0;j<3;j++)
      {
        vtkMatrix->SetElement(i,j,m_Transform->GetRotationMatrix().GetVnlMatrix().get(i,j));   
      }
      vtkMatrix->SetElement(i,3,m_Transform->GetOffset()[i]);   
    }

    (*m_PropIt)->Transform->SetMatrix(vtkMatrix);

    // Rotate also the plane



    (*m_PropIt)->Reslicer->SetResliceTransform((*m_PropIt)->Transform);
    (*m_PropIt)->Reslicer->SetOutputExtent(0,m_ImData->GetLargestPossibleRegion().GetSize()[0]-1,0,0,0,m_ImData->GetLargestPossibleRegion().GetSize()[2]-1);
    //cutter->SetOutputSpacing(m_Image->GetSpacing()[0],m_Image->GetSpacing()[1],m_Image->GetSpacing()[2]);
    (*m_PropIt)->Reslicer->Update();
    vtkImageData * axial = (*m_PropIt)->Reslicer->GetOutput();
    (*m_PropIt)->Texture->SetInput(axial);



/*
    itk::Point<float,3> plan1;
    plan1[0] = 0;
    plan1[1] = m_AxialSlice;
    plan1[2] = 0;
    itk::Point<float,3> plan2;
    plan2[0] = m_Image->GetLargestPossibleRegion().GetSize()[0]-1;
    plan2[1] = m_AxialSlice;
    plan2[2] = 0;
    itk::Point<float,3> plan3;
    plan3[0] = 0;
    plan3[1] = m_AxialSlice;
    plan3[2] = m_Image->GetLargestPossibleRegion().GetSize()[2]-1;

    if(m_GUI)
      {
      itk::ScalableAffineTransform<float>::Pointer affine = itk::ScalableAffineTransform<float>::New();
      
      itk::Point<float,3> cor;
      cor[0] = m_SlicePosition[0];
      cor[1] = m_AxialSlice;
      cor[2] = m_SlicePosition[2];
      affine->SetMatrix(v1.GetMatrix()*v2.GetMatrix());
      affine->SetCenterOfRotationComponent(cor);

      plan1 = affine->TransformPoint(plan1);
      plan2 = affine->TransformPoint(plan2);
      plan3 = affine->TransformPoint(plan3);    
      }

    m_AxialPlane->SetOrigin(plan1[0],plan1[1],plan1[2]);
    m_AxialPlane->SetPoint1(plan2[0],plan2[1],plan2[2]);
    m_AxialPlane->SetPoint2(plan3[0],plan3[1],plan3[2]);
*/

/*
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
*/
  //if(m_ImData->GetLargestPossibleRegion().GetSize()[0]>1 && m_ImData->GetLargestPossibleRegion().GetSize()[1]>1)
  //  {
  // (*m_PropIt)->Axial->GetProperty()->SetOpacity(static_cast<VisualImageProperty *>((*m_CurrentObject)->GetProperty())->GetOpacity(m_Orientation));
    //(*m_PropIt)->AxialSlice = dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetSlicePosition(2);
    
  //   unsigned int slice = dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetSlicePosition(m_Orientation);
/*
     unsigned int widthAxis,heightAxis,depthAxis;
     switch(this->m_Orientation)
      {
      case 0:
        widthAxis = 1;
        heightAxis = 2;
        depthAxis = 0;
       (*m_PropIt)->AxialSect->SetVOI(slice,slice, 
                                        0, m_ImData->GetLargestPossibleRegion().GetSize()[1]-1, 
                                        0, m_ImData->GetLargestPossibleRegion().GetSize()[2]-1);

        break;
      case 1:
        widthAxis = 0;
        heightAxis = 2;
        depthAxis = 1;
       (*m_PropIt)->AxialSect->SetVOI(0, m_ImData->GetLargestPossibleRegion().GetSize()[0]-1, 
                              slice,slice, 0, m_ImData->GetLargestPossibleRegion().GetSize()[2]-1);
        break;
      case 2:
        widthAxis = 0;
        heightAxis = 1;
        depthAxis = 2;
        (*m_PropIt)->AxialSect->SetVOI(0, m_ImData->GetLargestPossibleRegion().GetSize()[0]-1,  
                                       0, m_ImData->GetLargestPossibleRegion().GetSize()[1]-1,    
                                       slice, slice);
        break;
      }
*/
     /*
    (*m_PropIt)->Axial->SetScale(m_ImData->GetSpacing()[0], m_ImData->GetSpacing()[1], m_ImData->GetSpacing()[2]);
    double* bb = (*m_PropIt)->Axial->GetBounds();

    std::cout << bb[0] << " : " << bb[1] << " : " << bb[2] << " : " << bb[3] << " : " << bb[4] << " : " << bb[5] << std::endl;
    (*m_PropIt)->AxialSect->SetVOI(bb[0],bb[1],bb[2],bb[3],bb[4],bb[5]);

    (*m_PropIt)->AxialSect->SetInput((*m_PropIt)->Vol);
    //(*m_PropIt)->AxialSect->Update();
    //vtkImageData* imageText = (*m_PropIt)->AxialSect->GetOutput();
    //imageText->Print(std::cout);
    //imageText->SetDimensions(m_ImData->GetLargestPossibleRegion().GetSize()[1],m_ImData->GetLargestPossibleRegion().GetSize()[2],0);

    (*m_PropIt)->AxialText->SetInput((*m_PropIt)->AxialSect->GetOutput());*/
    /*//(*m_PropIt)->AxialPlane->SetOrigin(0, 0, (*m_PropIt)->AxialSlice);
    //(*m_PropIt)->AxialPlane->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1],(*m_PropIt)->AxialSlice+m_ImData->GetOrigin()[2]);
    double origin[2];
    origin[0] = -((double)(m_ImData->GetLargestPossibleRegion().GetSize()[widthAxis])/2.0)+0.5;
    origin[1] = -((double)(m_ImData->GetLargestPossibleRegion().GetSize()[heightAxis])/2.0)+0.5;
    (*m_PropIt)->AxialPlane->SetOrigin(origin[0],origin[1],0);
    (*m_PropIt)->AxialPlane->SetPoint1(origin[0]+(double)m_ImData->GetLargestPossibleRegion().GetSize()[widthAxis]-1, origin[1],0);
    (*m_PropIt)->AxialPlane->SetPoint2(origin[0], origin[1]+(double)m_ImData->GetLargestPossibleRegion().GetSize()[heightAxis]-1,0);
   */ //(*m_PropIt)->Axial->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1], m_ImData->GetOrigin()[2]);
   // }
/*
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
*/
  //(*m_CurrentObject)->UpdateMTime();
}

template <class TImage>
void
ImageVTKRenderMethodSlicer<TImage>
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

  
template <class TImage>
void
ImageVTKRenderMethodSlicer<TImage>
::Update()
{ 
  m_CurrentObject = m_ObjectList.begin();

  m_PropIt = m_ImagePropList.begin();
  unsigned int id = 0;
  for(; m_CurrentObject!=m_ObjectList.end(); m_CurrentObject++)
  {
    bool first = false;
    m_ImData = dynamic_cast<ImageSpatialObjectType *>((*m_CurrentObject)->GetObject())->GetImage();
    /** First Time we add the object properties to the list */
    if(id>=m_ImagePropList.size())
    {
      ImagePropSlicer* prop = new ImagePropSlicer;
      prop->ID=id;
      prop->MTime = m_ImData->GetPipelineMTime();
      m_ImagePropList.push_back(prop);
      m_PropIt = std::find(m_ImagePropList.begin(),m_ImagePropList.end(),prop);
      first = true;
    }

    if((*m_CurrentObject)->IsModified())
      {
      bool shouldDraw = false;
      if(!first)
      {
        if((*m_PropIt)->MTime >= m_ImData->GetPipelineMTime()) // if the internal image is not modified we just redraw
        {
          (*m_PropIt)->MTime = m_ImData->GetPipelineMTime();
          this->Draw();
          shouldDraw = true;
        }
        else
        {
          (*m_PropIt)->MTime = m_ImData->GetPipelineMTime();  

          RemoveActor(*m_CurrentObject); // remove the actor before creating the new one.
          
          (*m_CurrentObject)->RemoveVTKDefined((*m_PropIt)->Actor);
          delete (*m_PropIt);
          (*m_PropIt) = new ImagePropSlicer;
        }
      }
       
      if(!shouldDraw)
      {   
        (*m_PropIt)->Vol->SetDimensions(m_ImData->GetLargestPossibleRegion().GetSize()[0],
                          m_ImData->GetLargestPossibleRegion().GetSize()[1],
                          m_ImData->GetLargestPossibleRegion().GetSize()[2]);
    
        (*m_PropIt)->Vol->SetSpacing(m_ImData->GetSpacing()[0],m_ImData->GetSpacing()[1],m_ImData->GetSpacing()[2]);

        vtkUnsignedCharArray* mScalars = vtkUnsignedCharArray::New();

        itk::ImageRegionConstIterator<ImageType> it(m_ImData, m_ImData->GetLargestPossibleRegion());
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
        }  
      
        //try to put the scalars into the box and render directly
        (*m_PropIt)->Vol->GetPointData()->SetScalars(mScalars);   
        //(*m_PropIt)->AxialPlane->SetXResovution(2);
        //(*m_PropIt)->AxialPlane->SetYResovution(2);

        if(!m_Plane)
          {
          std::cout << "ImageVTKRenderMethodSlicer::Update() : no plane set" << std::endl;
          return;
          }

        (*m_PropIt)->Texture->InterpolateOn();
        (*m_PropIt)->Mapper->SetColorModeToMapScalars();
        (*m_PropIt)->Mapper->SetInput(m_Plane->GetOutput());
        (*m_PropIt)->Mapper->ImmediateModeRenderingOn();
        (*m_PropIt)->Actor->SetMapper((*m_PropIt)->Mapper);
        (*m_PropIt)->Actor->SetTexture((*m_PropIt)->Texture);
        (*m_PropIt)->Actor->GetProperty()->SetOpacity(static_cast<VisualImageProperty *>((*m_CurrentObject)->GetProperty())->GetOpacity(1));
        (*m_PropIt)->Actor->GetProperty()->SetInterpolationToFlat();
        (*m_PropIt)->Actor->SetScale(m_ImData->GetSpacing()[0], m_ImData->GetSpacing()[1], m_ImData->GetSpacing()[2]);
        //(*m_PropIt)->Axial->SetOrigin(m_ImData->GetOrigin()[0], m_ImData->GetOrigin()[1], m_ImData->GetOrigin()[2]);
 
        (*m_PropIt)->Vol->SetScalarTypeToShort();
        (*m_PropIt)->Reslicer->SetInput((*m_PropIt)->Vol);


        (*m_CurrentObject)->AddVTKDefined((*m_PropIt)->Actor); 
        m_ActorList.push_back((*m_PropIt)->Actor);
        m_ActorToBeAddedList.push_back((*m_PropIt)->Actor);
 
        mScalars->Delete();
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

    this->Draw();
    id++;
    m_PropIt++;
  }

}

} // end of namespace sov
