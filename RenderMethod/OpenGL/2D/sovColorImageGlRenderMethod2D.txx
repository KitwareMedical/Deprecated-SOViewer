/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovColorImageGlRenderMethod2D.txx,v $
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
#include "sovColorImageGlRenderMethod2D.h"

#include <itkImage.h>
#include <itkImageSpatialObject.h>
#include <itkImageRegionIterator.h>
#include "sovOpenGL.h"
#include <itkMinimumMaximumImageFilter.h>

namespace sov
{

template <class TImage>
ColorImageGlRenderMethod2D<TImage>
::ColorImageGlRenderMethod2D( void )
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
  cFlipY[0] = true;
  cFlipY[1] = true;
  
  cColorTable = ColorTableType::New();
  cColorTable->useDiscrete();
  cWinImData = NULL;
  cWinZBuffer = NULL;
  AddSupportedType("ImageSpatialObject");
}



template <class TImage>
ColorImageGlRenderMethod2D<TImage>
::~ColorImageGlRenderMethod2D( void )
{

}

  
template <class TImage>
void
ColorImageGlRenderMethod2D<TImage>
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

    cWinCenter[0] = m_DimSize[0]/2;
    cWinCenter[1] = m_DimSize[1]/2;


    glShadeModel(GL_FLAT);
    glClear(GL_COLOR_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();  
    glMatrixMode(GL_PROJECTION);

    m_SliceSizeX = m_DimSize[0];
    m_SliceSizeY = m_DimSize[1];

    float scale0 = m_Width/(float)m_SliceSizeX * cWinZoom
      * fabs(m_Spacing[0])/fabs(m_Spacing[0]);
    float scale1 = m_Height/(float)m_SliceSizeY  * cWinZoom
      * fabs(m_Spacing[1])/fabs(m_Spacing[0]);
 
    unsigned int cW = m_Width;
    unsigned int cH = m_Height;

    
    cWinOrientation = 0;

    glRasterPos2i((cFlipX[cWinOrientation])?cW:0,
                 (cFlipY[cWinOrientation])?cH:0); 
    
    glPixelZoom((cFlipX[cWinOrientation])?-scale0:scale0,
               (cFlipY[cWinOrientation])?-scale1:scale1);
   
    if( cValidImData && cViewImData )
    {
     glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDrawPixels(m_DimSize[0], m_DimSize[1], GL_RGBA, 
        GL_UNSIGNED_BYTE, cWinImData);
      glDisable(GL_BLEND);
    }
    
  }  
  //this->Update2();
}

template <class TImage>
void
ColorImageGlRenderMethod2D<TImage>
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}


template <class TImage>
void
ColorImageGlRenderMethod2D<TImage>
::Update()
{

  ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
  for(; currentObject!=lastObject; currentObject++)
  {
    typedef ImageSpatialObject<2,ImagePixelType> ImageSpatialObjectType;
    m_ImData = dynamic_cast<ImageSpatialObjectType *>((*currentObject)->GetObject())->GetImage();

    RegionType region = m_ImData->GetLargestPossibleRegion();
    if( region.GetNumberOfPixels() == 0 ) 
    {
      return;
    }

    m_DimSize[0]=m_ImData->GetLargestPossibleRegion().GetSize()[0];
    m_DimSize[1]=m_ImData->GetLargestPossibleRegion().GetSize()[1];
    m_Spacing[0]=m_ImData->GetSpacing()[0];
    m_Spacing[1]=m_ImData->GetSpacing()[1];

/*
   typedef itk::MinimumMaximumImageFilter<ImageType> MinMaxFilterType;
   typename MinMaxFilterType::Pointer minMaxFilter = MinMaxFilterType::New();

    minMaxFilter->SetInput(m_ImData);
    minMaxFilter->Update();
    
    cIWMin      = minMaxFilter->GetMinimum();
    cIWMax      = minMaxFilter->GetMaximum();
*/
    cIWModeMin  = IW_MIN2D;
    cIWModeMax  = IW_MAX2D;
    
    cImageMode = IMG_VAL2D;
    
    cWinZoom = 1;
    cWinOrientation = 2;
    cWinCenter[0] = m_DimSize[0]/2;
    cWinCenter[1] = m_DimSize[1]/2;

    cWinMinX  = 0;
    cWinSizeX = m_DimSize[0];
    if(cWinSizeX<m_DimSize[1])
    {
      cWinSizeX = m_DimSize[1];
    }

    cWinMaxX  = cWinSizeX - 1;
    cWinMinY  = 0;
    cWinSizeY = cWinSizeX;
    cWinMaxY  = cWinSizeY - 1;
    cWinDataSizeX = m_DimSize[0];
    cWinDataSizeY = m_DimSize[1];
    
    if(cWinImData != NULL)
    {
      delete [] cWinImData;
    }
    
    cWinImData = new unsigned char[ cWinDataSizeX * cWinDataSizeY*4 ];
    
    if(cWinZBuffer != NULL) 
    {
      delete [] cWinZBuffer;
    }
    
    cWinZBuffer = new unsigned short[ cWinDataSizeX * cWinDataSizeY ];
    cViewImData  = true;
    cValidImData = true;

  }

  this->Update2();
}

template <class TImage>
void
ColorImageGlRenderMethod2D<TImage>
::Update2()
{

  if( !cValidImData ) 
  {
    return;
  }
  
  int winWidth = (int)( m_DimSize[ 0 ] / cWinZoom );
  cWinSizeX = ( (int) winWidth);
  int ti = (int)( (int)cWinCenter[ 0 ] - winWidth/2);
  if( ti <= - (int) m_DimSize[ 0 ] ) 
    {
    ti = -(int)m_DimSize[ 0 ] + 1;
    }
  else if( ti >= (int)m_DimSize[ 0 ]) 
    {
    ti = m_DimSize[ 0 ] - 1;
    }
  cWinMinX = ti;
  cWinMaxX = m_DimSize[ 0 ] - 1; // here
  if( cWinMaxX >= static_cast<int>( m_DimSize[ 0 ] ) )
    {
    cWinMaxX = m_DimSize[ 0 ] - 1;
    }
  
  winWidth = static_cast<int>( m_DimSize[ 1 ] / cWinZoom );
  cWinSizeY = ( static_cast<int>( winWidth) );
  ti = static_cast<int>( static_cast<int>(cWinCenter[ 1 ]) - winWidth/2);
  if( ti <= - static_cast<int>( m_DimSize[ 1 ] ) ) 
    {
    ti = -(int)m_DimSize[ 1 ] + 1;
    }
  else if( ti >= static_cast<int>(m_DimSize[ 1 ] ) ) 
    {
    ti = m_DimSize[ 1 ] - 1;
    } 
  cWinMinY = ti;
  cWinMaxY = m_DimSize[ 1 ] - 1; // here
  if( cWinMaxY >= static_cast<int>( m_DimSize[ 1 ] ) ) 
    {
    cWinMaxY = m_DimSize[ 1 ] - 1;
    }
  
  memset( cWinImData, 0, cWinDataSizeX*cWinDataSizeY*4);
  if( cValidOverlayData ) 
    {
    memset(cWinOverlayData, 0, cWinDataSizeX*cWinDataSizeY*4);
    }
  
  IndexType ind;
  
  int l, m;
  
//  float tf;
  
  //ind[ cWinOrder[ 2 ] ] = cWinCenter[ cWinOrder[ 2 ] ];
  int startK = cWinMinY;
  if(startK<0)
    startK = 0;
  int startJ = cWinMinX;
  if(startJ<0)
    startJ = 0;
  for(int k=startK; k <= cWinMaxY; k++)
    {
    ind[1] = k;
    
    if(k-cWinMinY >= (int)cWinDataSizeY)
      continue;

    for(int j=startJ; j <= cWinMaxX; j++) 
      {
      ind[0] = j;
      
      if(j-cWinMinX >= (int)cWinDataSizeX)
         continue;
/*
      switch( cImageMode ) 
        {
        default:
        case IMG_VAL2D:
          tf = (float)((m_ImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
          break;
        case IMG_INV2D:
          tf = (float)((cIWMax-m_ImData->GetPixel(ind))/(cIWMax-cIWMin)*255);
          break;
        case IMG_LOG2D:
          tf = (float)(log(m_ImData->GetPixel(ind)-cIWMin+0.00000001)
            /log(cIWMax-cIWMin+0.00000001)*255);
          break;
        case IMG_DX2D:
          if(ind[0]>0) 
            {
            tf = (float)((m_ImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[0]--;
            tf -= (float)((m_ImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[0]++;
            tf += 128;
            } 
          else
            {
            tf = 128;
            }
          break;
        case IMG_DY2D:
          if(ind[1]>0) 
            {
            tf = (float)((m_ImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[1]--;
            tf -= (float)((m_ImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[1]++;
            tf += 128;
            }
          else
            {
            tf = 128;
            }
          break;
        case IMG_DZ2D:
          if(ind[2]>0) 
            {
            tf = (float)((m_ImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[2]--;
            tf -= (float)((m_ImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[2]++;
            tf += 128;
            }
          else
            {
            tf = 128;
            }
          break;
        }
      
      if( tf > 255 )
        {
        switch(cIWModeMax) 
          {
          case IW_MIN2D:
            tf = 0;
            break;
          default:
          case IW_MAX2D:
            tf = 255;
            break;
          case IW_FLIP2D:
            tf = 512-tf;
            if(tf<0) 
              {
              tf = 0;
              }
            break;
          }
        }
      else 
        {
        if( tf < 0 )
          {
          switch(cIWModeMin) 
            {
            default:
            case IW_MIN2D:
              tf = 0;
              break;
            case IW_MAX2D:
              tf = 255;
              break;
            case IW_FLIP2D:
              tf = -tf;
              if(tf>255)
                {
                tf = 255;
                }
              break;
            }
          }
        }*/
      
      l = (j-cWinMinX)*4 + (k-cWinMinY)*cWinDataSizeX*4;

      cWinImData[l] = (unsigned char)m_ImData->GetPixel(ind).GetRed();
      cWinImData[l+1] = (unsigned char)m_ImData->GetPixel(ind).GetGreen();
      cWinImData[l+2] = (unsigned char)m_ImData->GetPixel(ind).GetBlue();
      cWinImData[l+3] = 1*255; //overlay

      if( cValidOverlayData ) 
        {
        l = l * 4;
        if(cImageMode == IMG_MIP2D)
          {
          /*ind[cWinOrder[2]] = cWinZBuffer[(j-cWinMinX) + 
            (k-cWinMinY)*cWinDataSizeX];*/
          }
        
        if( sizeof( OverlayPixelType ) == 1 )
          {
          m = (int)*((unsigned char *)&(cOverlayData->GetPixel(ind)));
          if( m > 0 ) {
            m = m - 1;
            cWinOverlayData[l+0] = 
              (unsigned char)(cColorTable->color(m)->GetRed()*255);
            cWinOverlayData[l+1] = 
              (unsigned char)(cColorTable->color(m)->GetGreen()*255);
            cWinOverlayData[l+2] = 
              (unsigned char)(cColorTable->color(m)->GetBlue()*255);
            cWinOverlayData[l+3] = 
              (unsigned char)(cOverlayOpacity*255);
            }
          }
        else 
          {
          if(((unsigned char *)&(cOverlayData->GetPixel(ind)))[0]
            + ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1]
            + ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2] > 0)
            {
            if( sizeof( OverlayPixelType ) == 3 )
              {
              cWinOverlayData[l+0] = 
                ((unsigned char *)&(cOverlayData->GetPixel(ind)))[0];
              cWinOverlayData[l+1] = 
                ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1];
              cWinOverlayData[l+2] = 
                ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2];
              cWinOverlayData[l+3] = 
                (unsigned char)(cOverlayOpacity*255);
              }
            else 
              {
              if( sizeof( OverlayPixelType ) == 4 ) 
                {
                cWinOverlayData[l+0] = 
                  ((unsigned char *)&(cOverlayData->GetPixel(ind)))[0];
                cWinOverlayData[l+1] = 
                  ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1];
                cWinOverlayData[l+2] = 
                  ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2];
                cWinOverlayData[l+3] = 
                  (unsigned char)(((unsigned char *)
                  &(cOverlayData->GetPixel(ind)))[3]*cOverlayOpacity);
                }
              }
            }
          }
        }
    }
  }
}


} // end of namespace sov
