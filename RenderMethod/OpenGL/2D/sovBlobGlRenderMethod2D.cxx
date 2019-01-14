/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobGlRenderMethod2D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-03-14 21:44:51 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovBlobGlRenderMethod2D.h"

#include <itkBlobSpatialObject.h>
#include "sovOpenGL.h"

namespace sov
{

BlobGlRenderMethod2D
::BlobGlRenderMethod2D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("BlobSpatialObject");
  m_GreyTexture = new GreyTextureType;
  m_GreyTexture->SetType(GL_UNSIGNED_BYTE);
  m_GreyTexture->SetFormat(GL_RGBA);
  m_GreyTexture->SetComponents(4);
}

BlobGlRenderMethod2D
::~BlobGlRenderMethod2D( void )
{
  if(m_DisplaySlice)
  {
    delete [] m_DisplaySlice;
  }
  delete m_GreyTexture;
}

void
BlobGlRenderMethod2D
::draw( void )
{
   ObjectListType::iterator            currentObject,lastObject;
 
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  /** Reallocate the image volume */
  if(!m_DisplaySlice)
  {
    //m_DisplaySlice = new unsigned char [m_SliceSizeX*m_SliceSizeY*4];
    m_DisplaySliceImage = ImageType::New();
    ImageType::RegionType region;
    ImageType::SizeType size;
    size[0] = m_SliceSizeX;
    size[1] = m_SliceSizeY;
    region.SetSize(size);
    m_DisplaySliceImage->SetRegions(region);
    m_DisplaySliceImage->Allocate();
    PixelType pix;
    pix.SetAlpha(0);
    m_DisplaySliceImage->FillBuffer(pix);
  }

  //memset(m_DisplaySlice, 0, m_SliceSizeX*m_SliceSizeY*4);
  //double cOverlayOpacity = 0.7;

  for(; currentObject!=lastObject; currentObject++)
    {
    typedef itk::BlobSpatialObject<2> BlobType;
    BlobType::Pointer blob = static_cast<BlobType *>((*currentObject)->GetObject());


    BlobType::PointListType points = blob->GetPoints();
    BlobType::PointListType::const_iterator it = points.begin();

    while(it != points.end())
      {
      unsigned int i = (unsigned int)(blob->GetIndexToWorldTransform()
                                          ->TransformPoint(
                                            it->GetPosition())[0]);
      unsigned int j = (unsigned int)(blob->GetIndexToWorldTransform()
                                          ->TransformPoint(
                                            it->GetPosition())[1]);
      
       if (((unsigned int)(i)>0) && ((unsigned int)(i)<m_SliceSizeX) 
        && ((unsigned int)(j)>0) && ((unsigned int)(j)<m_SliceSizeY) )
          {
/*
          m_DisplaySlice[4*(i*m_SliceSizeX+j)] = 
                        (unsigned char)(blob->GetProperty()->GetRed()*255);
          m_DisplaySlice[4*(i*m_SliceSizeX+j)+1] = 
                                  (unsigned char)(blob->GetProperty()->GetGreen()*255);
          m_DisplaySlice[4*(i*m_SliceSizeX+j)+2] = 
                                 (unsigned char)(blob->GetProperty()->GetBlue()*255);
          m_DisplaySlice[4*(i*m_SliceSizeX+j)+3] = 
                                 (unsigned char)(cOverlayOpacity*255);     
  */        
          PixelType pix;
          pix.SetRed((unsigned char)((*it).GetRed()*255));
          pix.SetGreen((unsigned char)((*it).GetGreen()*255));
          pix.SetBlue((unsigned char)((*it).GetBlue()*255));
          pix.SetAlpha((unsigned char)((*currentObject)->GetProperty()->GetOpacity()*255));
          ImageType::IndexType ind;
          ind[0] = i;
          ind[1] = j;
          m_DisplaySliceImage->SetPixel(ind,pix);
         }

      it++;
      }
      //cOverlayOpacity= (*currentObject)->GetProperty()->GetOpacity();
    }

  m_GreyTexture->SetImage(m_DisplaySliceImage.GetPointer());
    // Turn on alpha blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  m_GreyTexture->Draw();
  //m_GreyTexture->DrawTransparent(cOverlayOpacity*255);

/* 
  double cWinZoom = 1;
  double m_Spacing[2];
  unsigned int cFlipX[2];
  unsigned int cFlipY[2];
 
  for(unsigned int i=0;i<2;i++)
  {
    m_Spacing[i]=1;
    cFlipX[i]=0;
    cFlipY[i]=0;
  }

  float scale0 = m_Width/(float)m_SliceSizeX * cWinZoom
    * fabs(m_Spacing[0])/fabs(m_Spacing[0]);
  float scale1 = m_Height/(float)m_SliceSizeY * cWinZoom
    * fabs(m_Spacing[1])/fabs(m_Spacing[0]);
 
  unsigned int cW = m_Width;
  unsigned int cH = m_Height;

  glRasterPos2i((cFlipX[m_Orientation])?cW:0,
                (cFlipY[m_Orientation])?cH:0); 
    
  glPixelZoom((cFlipX[m_Orientation])?-scale0:scale0,
              (cFlipY[m_Orientation])?-scale1:scale1);

  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawPixels(m_SliceSizeX, m_SliceSizeY, GL_RGBA, 
            GL_UNSIGNED_BYTE, m_DisplaySlice);
  glDisable(GL_BLEND);*/
   
}

void
BlobGlRenderMethod2D
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}

void
BlobGlRenderMethod2D
::Update()
{
  this->draw();
}

} // end of namespace sov
