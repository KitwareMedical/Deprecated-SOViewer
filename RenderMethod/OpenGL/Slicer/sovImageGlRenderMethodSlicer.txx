/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageGlRenderMethodSlicer.txx,v $
  Language:  C++
  Date:      $Date: 2006-04-17 18:13:14 $
  Version:   $Revision: 1.4 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovImageGlRenderMethodSlicer.h"
#include <itkImageRegionIteratorWithIndex.h>

#include <itkImage.h>
#include <itkImageSpatialObject.h>
#include <itkImageRegionIterator.h>
#include <itkMinimumMaximumImageFilter.h>
#include <itkExtractImageFilter.h>

#  if defined(WIN32) && !defined(__CYGWIN__)
#    include <windows.h>
#  endif
#  ifndef APIENTRY
#    if defined(__CYGWIN__)
#      define APIENTRY __attribute__ ((__stdcall__))
#    else
#      define APIENTRY
#    endif
#  endif

#include "sovOpenGL.h"

#if HAS_GLUT
#include <GL/glut.h>
#endif

namespace sov
{


template <class TImage>
ImageGlRenderMethodSlicer<TImage>
::ImageGlRenderMethodSlicer( void )
{
  NotUpdated = false;
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

  m_SupportedTypeList->clear();
    
  cValidOverlayData     = false;
  cViewOverlayData      = false;
  cViewOverlayCallBack  = NULL;
  cOverlayOpacity       = 0.0;
  cWinOverlayData       = NULL;
  cViewAxisLabel = true;
  cViewOverlayData = true;
  cViewDetails = true;
  cViewCrosshairs = true;
  cViewValue = true;
  cFlipX[0] = false;
  cFlipX[1] = false;
  cFlipX[2] = false;
  cFlipY[0] = false;
  cFlipY[1] = false;
  cFlipY[2] = false;
  cFlipZ[0] = false;
  cFlipZ[1] = false;
  cFlipZ[2] = false;
  
  cViewImData  = false;
  cValidImData = false;

  cColorTable = ColorTableType::New();
  //cColorTable.useGray();
  cColorTable->useDiscrete();
  cWinImData = NULL;
  cWinZBuffer = NULL;
  AddSupportedType("ImageSpatialObject");
  m_Orientation = 0;
  m_DataMin = 0;
  m_DataMax = 0;
  cImageMode = sovIMG_VAL;

  // Grey texture object
  m_GreyTexture = new GreyTextureType;
 if(!strcmp(m_PixelType.c_str(),"short"))   
     {   
     m_GreyTexture->SetType(GL_SHORT);   
     }
  else if(!strcmp(m_PixelType.c_str(),"unsigned char"))   
     {   
     m_GreyTexture->SetType(GL_UNSIGNED_BYTE);   
     }

  m_ImData = NULL;
  m_Slice = NULL;
  m_SlicePosition = 0;
}

template <class TImage>
ImageGlRenderMethodSlicer<TImage>
::~ImageGlRenderMethodSlicer( void )
{
  delete m_GreyTexture;
}

template <class TImage>
void
ImageGlRenderMethodSlicer<TImage>
::DisplayText(int x, int y, const char *format)
{
#if HAS_GLUT
  if (x>=0 && y>=0)
    glRasterPos2i(x,y);
 
  for (int i=0;i<strlen(format);i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,format[i]); 
 
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
 
  glPopAttrib();
  glPopAttrib();
  glPopAttrib();
#endif
}

template <class TImage>
void
ImageGlRenderMethodSlicer<TImage>
::draw( void )
{
  if( !m_Slice ) 
    {
    return;
    }

  ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
    {  
    m_GreyTexture->Draw();
    /*m_GreyTexture->FlipX(this->m_FlipX);
    m_GreyTexture->FlipY(this->m_FlipY);

    sov::VisualImageProperty * property = static_cast<sov::VisualImageProperty *>((*currentObject)->GetProperty());

    switch( property->GetDrawMode() )
      {
      case sov::VisualImageProperty::OVERLAY_MODE:
        m_GreyTexture->DrawOverlay( property->GetColor()[0],
                                    property->GetColor()[1],
                                    property->GetColor()[2],
                                    property->GetOpacity(m_Orientation));
        break;

      case sov::VisualImageProperty::TRANSPARENT_MODE:
        m_GreyTexture->DrawTransparent((unsigned char)(property->GetOpacity(m_Orientation)*255));
        break;

      case sov::VisualImageProperty::DEFAULT_MODE:
      default:
        m_GreyTexture->Draw();
        break;
      }*/
    }  

}

template <class TImage>
void
ImageGlRenderMethodSlicer<TImage>
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}


template <class TImage>
void
ImageGlRenderMethodSlicer<TImage>
::Update()
{
  ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
  
  for(; currentObject!=lastObject; currentObject++)
  {
    if((*currentObject)->IsModified()
       || (m_SlicePosition != dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetSlicePosition(m_Orientation)))
      {

      typedef ImageSpatialObject<3,ImagePixelType> ImageSpatialObjectType;
      m_ImData = dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetImage();
  
      RegionType region = m_ImData->GetLargestPossibleRegion();
      if( region.GetNumberOfPixels() == 0 ) 
      {
        return;
      }

      // Extract the slice 
      typedef itk::ExtractImageFilter<ImageType,SliceType> FilterType;
      typename FilterType::Pointer filter = FilterType::New();
      filter->SetInput(m_ImData);

      typename ImageType::SizeType size = m_ImData->GetLargestPossibleRegion().GetSize();
      size[m_Orientation] = 0;
      typename ImageType::IndexType start = m_ImData->GetLargestPossibleRegion().GetIndex();
      m_SlicePosition= dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetSlicePosition(m_Orientation);
      start[m_Orientation] = m_SlicePosition;
      typename ImageType::RegionType desiredRegion;
      desiredRegion.SetSize(  size  );
      desiredRegion.SetIndex( start );
      filter->SetExtractionRegion(desiredRegion);
      filter->Update();

      m_Slice = filter->GetOutput();

      m_GreyTexture->SetImage(m_Slice);
      //m_GreyTexture->SetFlip( this->cFlipX[this->m_Orientation],
      //                        this->cFlipY[this->m_Orientation],
      //                        this->cFlipZ[this->m_Orientation]);
    
      }

    if((*currentObject)->IsPropertyModified())
      {
      // if the intensity range is not defined we compute the min max and set the value
      if(!static_cast<VisualImageProperty*>((*currentObject)->GetProperty())->GetUseIntensityRange())
        {
        typedef itk::MinimumMaximumImageFilter<ImageType> MinMaxFilterType;
        typename MinMaxFilterType::Pointer minMaxFilter = MinMaxFilterType::New();
        minMaxFilter->SetInput(m_ImData);
        minMaxFilter->Update();
        static_cast<VisualImageProperty*>((*currentObject)->GetProperty())->SetUseIntensityRange(true);
        static_cast<VisualImageProperty*>((*currentObject)->GetProperty())->SetIntensityRange(minMaxFilter->GetMinimum(),minMaxFilter->GetMaximum());
        }

      double min = static_cast<VisualImageProperty*>((*currentObject)->GetProperty())->GetIntensityRange()[0];
      double max = static_cast<VisualImageProperty*>((*currentObject)->GetProperty())->GetIntensityRange()[1];

      m_GreyTexture->SetMininum((ImagePixelType)min);
      m_GreyTexture->SetMaximum((ImagePixelType)max);
   
      (*currentObject)->UpdateObjectPropertyMTime();
      //m_GreyTexture->Draw();
      }

    (*currentObject)->UpdateMTime();
  }
}

} // end of namespace sov
