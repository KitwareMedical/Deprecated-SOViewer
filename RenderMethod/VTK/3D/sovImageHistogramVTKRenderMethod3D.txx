/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageHistogramVTKRenderMethod3D.txx,v $
  Language:  C++
  Date:      $Date: 2005-05-12 10:22:01 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovImageHistogramVTKRenderMethod3D.h"

namespace sov
{

/** Constructor */
template <class TImage>
ImageHistogramVTKRenderMethod3D<TImage>
::ImageHistogramVTKRenderMethod3D()
{
  this->m_SupportedTypeList->clear();
  AddSupportedType("ImageSpatialObject");

  this->m_CutterPlane = vtkPlane::New();
  this->m_CutterPlane->SetNormal(0,1,0);

  this->m_Cutter = vtkCutter::New();
  this->m_Cutter->SetCutFunction(this->m_CutterPlane);
  this->m_Cutter->GenerateCutScalarsOff();

  this->m_CutterWrap = vtkTubeFilter::New();
  this->m_CutterWrap->SetInput(this->m_Cutter->GetOutput());
  this->m_CutterWrap->SetRadius(0.5);
  this->m_CutterWrap->SetNumberOfSides(6);
  this->m_CutterWrap->SidesShareVerticesOn();
  this->m_CutterWrap->CappingOn();

  this->m_GridGeometryFilter = vtkStructuredGridGeometryFilter::New();
  
  this->m_GridSmoothingFilter = vtkSmoothPolyDataFilter::New();
  this->m_GridSmoothingFilter->SetInput(this->m_GridGeometryFilter->GetOutput());
}

/** Destructor */
template <class TImage>
ImageHistogramVTKRenderMethod3D<TImage>
::~ImageHistogramVTKRenderMethod3D()
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
template <class TImage>
void
ImageHistogramVTKRenderMethod3D<TImage>
::Update()
{
  ObjectListType::iterator currentObject = m_ObjectList.begin();
  ObjectListType::iterator lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
    {
    if( (*currentObject)->IsModified() || (*currentObject)->IsPropertyAskingForUpdate() )
      {
      //remove all actors that need to be removed...

      std::list< Property * >::iterator it = this->m_PropertyList.begin();
      std::list< Property * >::iterator end = this->m_PropertyList.end();

      for(; it!=end; it++ )
        {
        if( (*it)->m_Object == (*currentObject)->GetObject() )
          {
          m_ActorToBeRemovedList.push_back((*it)->m_OutlineActor);
          m_ActorToBeRemovedList.push_back((*it)->m_HistogramActor);
          m_ActorToBeRemovedList.push_back((*it)->m_CutterActor);
          this->m_PropertyList.remove(*it);
          break;
          }
        }

      //generate the new histogram...

      VisualImageProperty * property = static_cast<VisualImageProperty*>((*currentObject)->GetProperty());

      unsigned int  minimumIntensity = property->GetIntensityRange()[0];
      unsigned int  maximumIntensity = property->GetIntensityRange()[1];
      unsigned int  minimumFrequency = property->GetFrequencyRange()[0];
      unsigned int  maximumFrequency = property->GetFrequencyRange()[1];
      unsigned int  numberOfBins = property->GetNumberOfBins();
      unsigned int  smoothingIterations = property->GetSmoothingIterations();
      float         smoothingFactor = property->GetSmoothingFactor();
      unsigned int  offset=0;
      float         coord[3]={0,0,0};
      unsigned int  slice=0;
      unsigned int  stepIndent = (unsigned int)property->GetRenderingResolution();
      unsigned int  step = 0;
      unsigned int  scalar;
      float         y;

      ImageConstPointer src = dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetImage();

      itk::ImageSliceConstIteratorWithIndex<ImageType> srcIt(
              src,
              src->GetLargestPossibleRegion());     

      ListSamplePointer sample = ListSampleType::New();

      HistogramConstPointer histogram = HistogramType::New();

      typename HistogramType::SizeType bins;
      bins.Fill(numberOfBins);

      HistogramGeneratorPointer generator = HistogramGeneratorType::New();
      generator->SetListSample(sample);
      generator->SetMarginalScale((maximumIntensity-minimumIntensity)/(float)numberOfBins);
      generator->SetNumberOfBins(bins);
      
      srcIt.SetFirstDirection(0);
      srcIt.SetSecondDirection(1);
      srcIt.GoToBegin();

      vtkFloatArray *scalars = vtkFloatArray::New();
      scalars->SetNumberOfComponents(1);
      scalars->SetNumberOfTuples(numberOfBins*src->GetLargestPossibleRegion().GetSize()[2]);

      vtkPoints *points = vtkPoints::New();
      points->Allocate(numberOfBins*src->GetLargestPossibleRegion().GetSize()[2]);

      std::cout<<"Processing ["<<(*currentObject)<<"]...";

      while(!srcIt.IsAtEnd())
        {
        while(!srcIt.IsAtEndOfSlice())
          {
          while( !srcIt.IsAtEndOfLine() )
            {
            if( step%stepIndent == 0 )
              {
              if(  srcIt.Value() > minimumIntensity
                && srcIt.Value() < maximumIntensity )
                {
                sample->PushBack(srcIt.Value());
                }
              }
            ++step;
            ++srcIt;
            }
          srcIt.NextLine();
          }     
        srcIt.NextSlice();
        
        step=0;

        generator->Update();
        histogram = generator->GetOutput();
        sample->Clear();

        for( unsigned int x=0; x<numberOfBins; x++ )
          {
          offset = x+(numberOfBins*slice);

          scalar = histogram->GetFrequency(x,0);

          if( scalar > maximumFrequency )
            {
            scalar = maximumFrequency;
            }

          if( scalar < minimumFrequency )
            {
            scalar = minimumFrequency;
            }

          y = (float)scalar/(float)maximumFrequency;
          
          scalars->InsertTuple1(offset,y);

          coord[0]=x;
          coord[1]=y;
          coord[2]=slice;

          points->InsertPoint(offset,coord);
          }
        
        slice++;
        }
      
      Property * prop = new Property();
      prop->m_Object = (*currentObject)->GetObject();
      prop->m_Grid->SetDimensions(  numberOfBins,
                                    src->GetLargestPossibleRegion().GetSize()[2]
                                    ,1);
      prop->m_Grid->SetPoints(points);
      prop->m_Grid->GetPointData()->SetScalars(scalars);

      points->Delete();
      scalars->Delete();

      std::cout<<"Done !!!"<<std::endl;

      this->m_GridGeometryFilter->SetInput(prop->m_Grid);
      this->m_GridGeometryFilter->SetExtent(  0,numberOfBins-1,
                                              0,src->GetLargestPossibleRegion().GetSize()[2],
                                              0,0);
      this->m_GridGeometryFilter->Update();
      
      this->m_GridSmoothingFilter->SetNumberOfIterations(smoothingIterations);
      this->m_GridSmoothingFilter->SetRelaxationFactor(smoothingFactor);
      this->m_GridSmoothingFilter->Update();

      prop->m_HistogramActor->SetScale( 1,
                                        property->GetFrequencyScale(),
                                        1);
      prop->m_HistogramPolyData = this->m_GridSmoothingFilter->GetOutput();
      prop->m_HistogramMapper->SetInput(this->m_GridSmoothingFilter->GetOutput());
      prop->m_HistogramLookupTable->SetHueRange(        property->GetHueRange()[0],
                                                        property->GetHueRange()[1]);
      prop->m_HistogramLookupTable->SetAlphaRange(      property->GetAlphaRange()[0],
                                                        property->GetAlphaRange()[1]);
      prop->m_HistogramLookupTable->SetSaturationRange( property->GetSaturationRange()[0],
                                                        property->GetSaturationRange()[1]);
      prop->m_HistogramLookupTable->Build();
      
      this->m_CutterPlane->SetOrigin( 0,
                                      property->GetIsoFrequency()/(float)maximumIntensity,
                                      0);
      this->m_Cutter->SetInput(this->m_GridSmoothingFilter->GetOutput());
      this->m_CutterWrap->Update();

      prop->m_CutterMapper->SetInput(this->m_CutterWrap->GetOutput());
      prop->m_CutterActor->SetPosition(0,
                                       property->GetFrequencyScale()*(property->GetIsoFrequency()/(float)property->GetFrequencyRange()[1])
                                       ,0);
      prop->Update();

      m_ActorToBeAddedList.push_back(prop->m_HistogramActor);
      
      if( property->IsoFrequency() )
        {
        m_ActorToBeAddedList.push_back(prop->m_CutterActor);
        }
      
      if( property->BoundingBox() )
        {
        m_ActorToBeAddedList.push_back(prop->m_OutlineActor);
        }

      this->m_PropertyList.push_back(prop);

      (*currentObject)->UpdatePropertyMTime();
      }
    else
      {
      ImageConstPointer src = dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetImage();
      VisualImageProperty * property = static_cast<VisualImageProperty*>((*currentObject)->GetProperty());

      std::list< Property * >::iterator it = this->m_PropertyList.begin();
      std::list< Property * >::iterator end = this->m_PropertyList.end();

      for(; it!=end; it++ )
        {
        if( (*it)->m_Object == (*currentObject)->GetObject() )
          {
          (*it)->m_HistogramActor->SetScale(1,
                                            property->GetFrequencyScale(),
                                            1);
          (*it)->m_HistogramLookupTable->SetHueRange(         property->GetHueRange()[0],
                                                              property->GetHueRange()[1]);
          (*it)->m_HistogramLookupTable->SetAlphaRange(       property->GetAlphaRange()[0],
                                                              property->GetAlphaRange()[1]);
          (*it)->m_HistogramLookupTable->SetSaturationRange(  property->GetSaturationRange()[0],
                                                              property->GetSaturationRange()[1]);
          (*it)->m_HistogramLookupTable->Build();

          this->m_CutterPlane->SetOrigin( 0,
                                          property->GetIsoFrequency()/(float)property->GetFrequencyRange()[1],
                                          0);
          this->m_Cutter->SetInput((*it)->m_HistogramPolyData);
          this->m_CutterWrap->Update();

          (*it)->m_CutterMapper->SetInput(this->m_CutterWrap->GetOutput());
          (*it)->m_CutterActor->SetPosition(0,
                                            property->GetFrequencyScale()*(property->GetIsoFrequency()/(float)property->GetFrequencyRange()[1])
                                            ,0);
          if( property->IsoFrequency() )
            {
            m_ActorToBeAddedList.push_back((*it)->m_CutterActor);
            }
          else
            {
            m_ActorToBeRemovedList.push_back((*it)->m_CutterActor);
            }
          
          if( property->BoundingBox() )
            {
            m_ActorToBeAddedList.push_back((*it)->m_OutlineActor);
            }          
          else
            {
            m_ActorToBeRemovedList.push_back((*it)->m_OutlineActor);
            }

          (*it)->Update();
          }
        }
      }
    }
}

/** Print the method */
template <class TImage>
void
ImageHistogramVTKRenderMethod3D<TImage>
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}


} // end of namespace sov
