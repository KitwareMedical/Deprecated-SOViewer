/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: OpenGLSliceTexture.txx,v $
  Language:  C++
  Date:      $Date: 2006-04-21 22:33:46 $
  Version:   $Revision: 1.2 $
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include <itkRGBPixel.h>
#include "OpenGLSliceTexture.h"

#if defined(_MSC_VER)
#pragma warning ( disable : 4756 )
#endif

template<class TPixel>
OpenGLSliceTexture<TPixel>
::OpenGLSliceTexture()
{
  // Set to -1 to force a call to 'generate'
  m_IsTextureInitalized = false;

  // Create the filter
  m_PadFilter = FilterType::New();

  // Set the update time to -1
  m_UpdateTime = 0;

  // Init the GL settings to uchar, luminance defaults, which are harmless
  m_GlComponents = 1;
  m_GlFormat = GL_LUMINANCE;
  m_GlType = GL_SHORT;
  m_Maximum = itk::NumericTraitsSOV<TPixel>::max();
  m_Minimum = 0;
  
  m_FlipX = false;
  m_FlipY = false;
  m_FlipZ = false;
}

template<class TPixel>
OpenGLSliceTexture<TPixel>
::~OpenGLSliceTexture()
{
  if(m_IsTextureInitalized)
    glDeleteTextures(1,&m_TextureIndex);
}

template<class TPixel>
void
OpenGLSliceTexture<TPixel>
::SetImage(ImagePointer inImage)
{
  if(m_Image != inImage)
  {
    m_Image = inImage;
    m_UpdateTime = 0;
  }
}

template<class TPixel>
void
OpenGLSliceTexture<TPixel>
::Update()
{
  // Better have an image
  assert(m_Image);

  // Update the image (necessary?)
  if(m_Image->GetSource())
    m_Image->GetSource()->UpdateLargestPossibleRegion();

  // Check if everything is up-to-date and no computation is needed
  //if (m_IsTextureInitalized && m_UpdateTime == m_Image->GetPipelineMTime())
  //  return;

  // Promote the image dimensions to powers of 2
  //itk::Size<2> szImage = m_Image->GetLargestPossibleRegion().GetSize();
  this->m_ImageSize = this->m_Image->GetLargestPossibleRegion().GetSize();

  this->m_TextureSize.Fill(1);

  // Use shift to quickly double the coordinates
  for(unsigned int i=0;i<2;i++)
    {
    while(this->m_TextureSize[i] < this->m_ImageSize[i])
      {
      this->m_TextureSize[i] <<= 1;
      }
    }

  // Compute the pad offset
  this->m_TextureOffset[0] = this->m_TextureSize[0]-this->m_ImageSize[0];
  this->m_TextureOffset[1] = this->m_TextureSize[1]-this->m_ImageSize[1];

  // Set the parameters of the pad filter
  m_PadFilter->SetInput(m_Image);
  m_PadFilter->SetPadUpperBound(m_TextureOffset.data_block());

  // Apply the padding
  m_PadFilter->UpdateLargestPossibleRegion();

  // Create the texture index if necessary
  if(!m_IsTextureInitalized)
    {
    // Generate one texture
    glGenTextures(1,&m_TextureIndex);
    m_IsTextureInitalized = true;
    }

    // compute the scale
  double Min = 0;
  double Max = itk::NumericTraitsSOV<TPixel>::max();

  double scale = 1;

  if((m_Maximum-m_Minimum)>0)
    {
    scale = (Max-Min)/((float)m_Maximum-(float)m_Minimum);
    }
  
  float shift = 0;
  if(fabs(Max-Min)>0)
    {
    shift = (Min - scale * m_Minimum)/(Max-Min); 
    }

  glPixelTransferf(GL_RED_BIAS ,shift);
  glPixelTransferf(GL_GREEN_BIAS ,shift);
  glPixelTransferf(GL_BLUE_BIAS ,shift);

  glPixelTransferf(GL_RED_SCALE ,scale);
  glPixelTransferf(GL_GREEN_SCALE ,scale);
  glPixelTransferf(GL_BLUE_SCALE ,scale);


  // Select the texture for pixel pumping
  glBindTexture(GL_TEXTURE_2D,m_TextureIndex);

  // Properties for the texture
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  
  // Pump the pixels into the texture
  glTexImage2D(GL_TEXTURE_2D,0,m_GlComponents,
               m_TextureSize[0],m_TextureSize[1],
               0,m_GlFormat,m_GlType,
               m_PadFilter->GetOutput()->GetBufferPointer());

  // Remember the image's timestamp
  m_UpdateTime = m_Image->GetPipelineMTime();
}

template<class TPixel>
void
OpenGLSliceTexture<TPixel>
::Draw()
{
  // Update the texture
  Update();

  // Should have a texture number
  assert(m_IsTextureInitalized);

  // GL settings
  glPushAttrib(GL_TEXTURE_BIT);
  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  glBlendFunc(GL_ONE, GL_ONE);

  // Select our texture
  glBindTexture(GL_TEXTURE_2D,m_TextureIndex);

  // Set the color to white
  glColor3ub(255,255,255);

  float textCoordX[2];
  textCoordX[0] = (this->m_FlipX==0?0:1.0-(m_TextureOffset[0]/(float)m_TextureSize[0]));
  textCoordX[1] = (this->m_FlipX==0?1.0-(m_TextureOffset[0]/(float)m_TextureSize[0]):0);
  
  float textCoordY[2];
  textCoordY[0] = (this->m_FlipY==0?0:1.0-(m_TextureOffset[1]/(float)m_TextureSize[1]));
  textCoordY[1] = (this->m_FlipY==0?1.0-(m_TextureOffset[1]/(float)m_TextureSize[1]):0);

  // Draw quad 
  glBegin(GL_QUADS);
    glTexCoord2d(textCoordX[0],textCoordY[0]);
    glVertex2d(0,0);
    glTexCoord2d(textCoordX[0],textCoordY[1]);
    glVertex2d(0,m_ImageSize[1]);
    glTexCoord2d(textCoordX[1],textCoordY[1]);
    glVertex2d(m_ImageSize[0],m_ImageSize[1]);
    glTexCoord2d(textCoordX[1],textCoordY[0]);
    glVertex2d(this->m_ImageSize[0],0);
  glEnd();

  glDisable(GL_BLEND);

  glPopAttrib();
}

template<class TPixel>
void
OpenGLSliceTexture<TPixel>
::DrawTransparent(unsigned char alpha)
{
  // Update the texture
  Update();

  // Should have a texture number
  assert(m_IsTextureInitalized);

  // GL settings
  glPushAttrib(GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);

  // Turn on alpha blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  // Select our texture
  glBindTexture(GL_TEXTURE_2D,m_TextureIndex);

  // Set the color to white
  glColor4ub(255,255,255,alpha);
    
  // Draw quad 
  glBegin(GL_QUADS);
  glTexCoord2d(0,0);
  glVertex2d(0,0);
  glTexCoord2d(0,1);
  glVertex2d(0,m_TextureSize[1]);
  glTexCoord2d(1,1);
  glVertex2d(m_TextureSize[0],m_TextureSize[1]);
  glTexCoord2d(1,0);
  glVertex2d(m_TextureSize[0],0);
  glEnd();

  glPopAttrib();
}

template<class TPixel>
void
OpenGLSliceTexture<TPixel>
::DrawOverlay(float r, float g, float b, float alpha)
{
  // Update the texture
  Update();

  // Should have a texture number
  assert(m_IsTextureInitalized);

  // GL settings
  glPushAttrib(GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);

  float textCoordX[2];
  textCoordX[0] = (this->m_FlipX==0?0:1.0-(m_TextureOffset[0]/(float)m_TextureSize[0]));
  textCoordX[1] = (this->m_FlipX==0?1.0-(m_TextureOffset[0]/(float)m_TextureSize[0]):0);
  
  float textCoordY[2];
  textCoordY[0] = (this->m_FlipY==0?0:1.0-(m_TextureOffset[1]/(float)m_TextureSize[1]));
  textCoordY[1] = (this->m_FlipY==0?1.0-(m_TextureOffset[1]/(float)m_TextureSize[1]):0);

  // Turn on alpha blending
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  
  glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);

  // Select our texture
  glBindTexture(GL_TEXTURE_2D,m_TextureIndex);

  // Set the color
  //glColor4ub(r,g,b,alpha);

  // Draw quad 
  glBegin(GL_QUADS);
  glColor4f(r,g,b,alpha);
  glTexCoord2d(textCoordX[0],textCoordY[0]);
  glVertex2d(0,0);
  glTexCoord2d(textCoordX[0],textCoordY[1]);
  glVertex2d(0,m_ImageSize[1]);
  glTexCoord2d(textCoordX[1],textCoordY[1]);
  glVertex2d(m_ImageSize[0],m_TextureSize[1]);
  glTexCoord2d(textCoordX[1],textCoordY[0]);
  glVertex2d(m_ImageSize[0],0);
  glEnd();
  
  glDisable(GL_BLEND);

  glPopAttrib();
}
