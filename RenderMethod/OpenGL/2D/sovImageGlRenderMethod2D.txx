/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageGlRenderMethod2D.txx,v $
  Language:  C++
  Date:      $Date: 2006-03-14 21:44:51 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovImageGlRenderMethod2D.h"

#include <itkImage.h>
#include <itkImageSpatialObject.h>
#include <itkImageRegionIterator.h>
#include "sovOpenGL.h"
#include <itkMinimumMaximumImageFilter.h>

namespace sov
{

template <class TImage>
ImageGlRenderMethod2D<TImage>
::ImageGlRenderMethod2D( void )
{
  m_SupportedTypeList->clear();
  m_PixelType = typeid(typename TImage::PixelType).name();
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
  cFlipY[0] = false;
  cFlipZ[0] = false;
  cFlipZ[1] = false;
  
  cColorTable = ColorTableType::New();
  cColorTable->useDiscrete();
  cWinImData = NULL;
  cWinZBuffer = NULL;
  AddSupportedType("ImageSpatialObject");
  m_GreyTexture = new GreyTextureType;
  if(!strcmp(m_PixelType.c_str(),"short"))   
     {   
     m_GreyTexture->SetType(GL_SHORT);   
     }
  else if(!strcmp(m_PixelType.c_str(),"unsigned char"))   
     {   
     m_GreyTexture->SetType(GL_UNSIGNED_BYTE);   
     } 
}



template <class TImage>
ImageGlRenderMethod2D<TImage>
::~ImageGlRenderMethod2D( void )
{
  delete m_GreyTexture;
}

  
template <class TImage>
void
ImageGlRenderMethod2D<TImage>
::draw( void )
{
  if( !m_ImData ) 
  {
     return;
  }

  ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
   
  for(; currentObject!=lastObject; currentObject++)
    {   
    m_GreyTexture->Draw();
    }  
}

template <class TImage>
void
ImageGlRenderMethod2D<TImage>
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}


template <class TImage>
void
ImageGlRenderMethod2D<TImage>
::Update()
{
 ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
  
  for(; currentObject!=lastObject; currentObject++)
  {

    if((*currentObject)->IsModified())
    {

      typedef ImageSpatialObject<2,ImagePixelType> ImageSpatialObjectType;
      m_ImData = dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetImage();
  
      // Compute the minimum and maximum value of the input image
      typedef itk::MinimumMaximumImageFilter<ImageType> MinMaxFilterType;
      typename MinMaxFilterType::Pointer minMaxFilter = MinMaxFilterType::New();
      minMaxFilter->SetInput(m_ImData);
      minMaxFilter->Update();
      m_GreyTexture->SetMininum(minMaxFilter->GetMinimum());
      m_GreyTexture->SetMaximum(minMaxFilter->GetMaximum());
  
      RegionType region = m_ImData->GetLargestPossibleRegion();
      if( region.GetNumberOfPixels() == 0 ) 
      {
        return;
      }

      m_GreyTexture->SetImage(m_ImData);
      }
    }
}


} // end of namespace sov
