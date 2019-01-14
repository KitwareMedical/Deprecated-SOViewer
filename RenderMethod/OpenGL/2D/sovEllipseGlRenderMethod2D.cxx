/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovEllipseGlRenderMethod2D.cxx,v $
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
#include "sovEllipseGlRenderMethod2D.h"

#include <itkEllipseSpatialObject.h>
#include "sovOpenGL.h"

namespace sov
{

EllipseGlRenderMethod2D
::EllipseGlRenderMethod2D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("EllipseSpatialObject");
  m_DisplaySlice = NULL;
  m_DisplaySliceImage = NULL;
  m_GreyTexture = new GreyTextureType;
  m_GreyTexture->SetType(GL_UNSIGNED_BYTE);
  m_GreyTexture->SetFormat(GL_RGB);
  m_GreyTexture->SetComponents(3);
}

EllipseGlRenderMethod2D
::~EllipseGlRenderMethod2D( void )
{
  if(m_DisplaySlice)
  {
    delete [] m_DisplaySlice;
  }

  delete m_GreyTexture;
}

void
EllipseGlRenderMethod2D
::draw( void )
{
  ObjectListType::iterator            currentObject,lastObject;
 
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  /** Reallocate the image volume */
  if(!m_DisplaySlice)
  {
    //m_DisplaySlice = new unsigned char [m_SliceSizeX*m_SliceSizeY*3];
    m_DisplaySliceImage = ImageType::New();
    ImageType::RegionType region;
    ImageType::SizeType size;
    size[0] = m_SliceSizeX;
    size[1] = m_SliceSizeY;
    region.SetSize(size);
    m_DisplaySliceImage->SetRegions(region);
    m_DisplaySliceImage->Allocate();
  }

  //memset(m_DisplaySlice, 0, m_SliceSizeX*m_SliceSizeY*3);

   double cOverlayOpacity = 0.7;

  for(; currentObject!=lastObject; currentObject++)
  {
    EllipseType::Pointer ellipse = static_cast<EllipseType *>((*currentObject)->GetObject());


    // Check the bounding box
    ellipse->ComputeBoundingBox();
    
    unsigned int minimimumx = (unsigned int)(ellipse->GetBoundingBox()->GetBounds()[0]);
    unsigned int maximimumx = (unsigned int)(ellipse->GetBoundingBox()->GetBounds()[1]);
    unsigned int minimimumy = (unsigned int)(ellipse->GetBoundingBox()->GetBounds()[2]);
    unsigned int maximimumy = (unsigned int)(ellipse->GetBoundingBox()->GetBounds()[3]);
    
    
    for(unsigned int i=minimimumx;i<=maximimumx;i++)
      {
      for(unsigned int j=minimimumy;j<=maximimumy;j++)
        {

        itk::Point<double,2> pt;
        pt[0]=i;
        pt[1]=j;


        if (ellipse->IsInside(pt)
        && ((unsigned int)(i)>0) && ((unsigned int)(i)<m_SliceSizeX) 
        && ((unsigned int)(j)>0) && ((unsigned int)(j)<m_SliceSizeY) )
          {

          /*m_DisplaySlice[4*(i*m_SliceSizeX+j)] = 
                        (unsigned char)(ellipse->GetProperty()->GetRed()*255);
          m_DisplaySlice[4*(i*m_SliceSizeX+j)+1] = 
                                  (unsigned char)(ellipse->GetProperty()->GetGreen()*255);
          m_DisplaySlice[4*(i*m_SliceSizeX+j)+2] = 
                                 (unsigned char)(ellipse->GetProperty()->GetBlue()*255);
          m_DisplaySlice[4*(i*m_SliceSizeX+j)+3] = 
                                 (unsigned char)(cOverlayOpacity*255); */
          
          PixelType pix;
          pix.SetRed((unsigned char)(ellipse->GetProperty()->GetRed()*255));
          pix.SetGreen((unsigned char)(ellipse->GetProperty()->GetGreen()*255));
          pix.SetBlue((unsigned char)(ellipse->GetProperty()->GetBlue()*255));
          ImageType::IndexType ind;
          ind[0] = i;
          ind[1] = j;
          m_DisplaySliceImage->SetPixel(ind,pix);
          }
        
        }

      }

    //cOverlayOpacity= (*currentObject)->GetProperty()->GetOpacity();

  }


  m_GreyTexture->SetImage(m_DisplaySliceImage.GetPointer());

  // Turn on alpha blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  m_GreyTexture->DrawTransparent((unsigned char)(cOverlayOpacity*255));

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
EllipseGlRenderMethod2D
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}

void
EllipseGlRenderMethod2D
::Update()
{

}

} // end of namespace sov
