/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLandmarkGlRenderMethod2D.cxx,v $
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
#include "sovLandmarkGlRenderMethod2D.h"

#include <itkLandmarkSpatialObject.h>
#include "sovOpenGL.h"

namespace sov
{

LandmarkGlRenderMethod2D
::LandmarkGlRenderMethod2D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("LandmarkSpatialObject");
  m_DisplaySlice = NULL;
  m_DisplayType = SOL_SQUARE;
  m_LandmarkSize = 6;
}

LandmarkGlRenderMethod2D
::~LandmarkGlRenderMethod2D( void )
{
  if(m_DisplaySlice)
  {
    delete [] m_DisplaySlice;
  }
}

void
LandmarkGlRenderMethod2D
::draw( void )
{
  ObjectListType::iterator            currentObject,lastObject;
//  LandmarkPointListType::iterator         currentLandmarkPoint,lastLandmarkPoint,test;
 
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
  
  /** Reallocate the image volume */
  if(!m_DisplaySlice)
  {
    m_DisplaySlice = new unsigned char [m_SliceSizeX*m_SliceSizeY*4];
  }

  memset(m_DisplaySlice, 0, m_SliceSizeX*m_SliceSizeY*4);

  for(; currentObject!=lastObject; currentObject++)
  {
    LandmarkType::Pointer Landmark = static_cast<LandmarkType *>((*currentObject)->GetObject());

    double cOverlayOpacity = 0.9;
       
    LandmarkType::PointListType::const_iterator it = Landmark->GetPoints().begin();
    
    while(it != Landmark->GetPoints().end())
    {

      unsigned int x = (unsigned int)(*it).GetPosition()[0];
      unsigned int y = (unsigned int)(*it).GetPosition()[1];
    
      switch(m_DisplayType)
      {
         case SOL_CIRCLE:               
                         for(double angle = 0; angle<2*3.141596;angle+=0.1)
                         {
                            unsigned int i = (unsigned int)(x+(unsigned int)(m_LandmarkSize*cos(angle)));
                            unsigned int j = (unsigned int)(y+(unsigned int)(m_LandmarkSize*sin(angle)));

                            if((i>0) && (j>0) && ((unsigned int)(i)<m_SliceSizeX) && ((unsigned int)(j)<m_SliceSizeY))
                            {
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)] = 
                                            (unsigned char)((*it).GetRed()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+1] = 
                                                      (unsigned char)((*it).GetGreen()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+2] = 
                                                      (unsigned char)((*it).GetBlue()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+3] = 
                                                     (unsigned char)(cOverlayOpacity*255);
                            }

                         }
                         break;
  
         case SOL_SQUARE:
                          for(int k=0;k<=m_LandmarkSize;k++)
                          {
                            unsigned int j = (unsigned int)(y-m_LandmarkSize/2+k);
                            unsigned int i = (unsigned int)(x-m_LandmarkSize/2);
                            
                            if((i>m_LandmarkSize) && (j>m_LandmarkSize) && (i<m_SliceSizeX-m_LandmarkSize) && (j<m_SliceSizeY-m_LandmarkSize))
                            {
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)] = 
                                            (unsigned char)((*it).GetRed()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+1] = 
                                                      (unsigned char)((*it).GetGreen()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+2] = 
                                                      (unsigned char)((*it).GetBlue()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+3] = 
                                                     (unsigned char)(cOverlayOpacity*255);
                              

                              j = (unsigned int)(y+m_LandmarkSize/2-k);
                              i = (unsigned int)(x+m_LandmarkSize/2);

                              m_DisplaySlice[4*(j*m_SliceSizeX+i)] = 
                                            (unsigned char)((*it).GetRed()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+1] = 
                                                      (unsigned char)((*it).GetGreen()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+2] = 
                                                      (unsigned char)((*it).GetBlue()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+3] = 
                                                     (unsigned char)(cOverlayOpacity*255);  

                              j = (unsigned int)(y-m_LandmarkSize/2);
                              i = (unsigned int)(x-m_LandmarkSize/2+k);

                              m_DisplaySlice[4*(j*m_SliceSizeX+i)] = 
                                            (unsigned char)((*it).GetRed()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+1] = 
                                                      (unsigned char)((*it).GetGreen()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+2] = 
                                                      (unsigned char)((*it).GetBlue()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+3] = 
                                                     (unsigned char)(cOverlayOpacity*255);  


                              j = (unsigned int)(y+m_LandmarkSize/2);
                              i = (unsigned int)(x+m_LandmarkSize/2-k);

                              m_DisplaySlice[4*(j*m_SliceSizeX+i)] = 
                                            (unsigned char)((*it).GetRed()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+1] = 
                                                      (unsigned char)((*it).GetGreen()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+2] = 
                                                      (unsigned char)((*it).GetBlue()*255);
                              m_DisplaySlice[4*(j*m_SliceSizeX+i)+3] = 
                                                     (unsigned char)(cOverlayOpacity*255);  

                            }
                          }

                          break;
         case SOL_POINT:
           {
             unsigned int i = y;
             unsigned int j = x;

              m_DisplaySlice[4*(i*m_SliceSizeX+j)] = 
                                        (unsigned char)((*it).GetRed()*255);
              m_DisplaySlice[4*(i*m_SliceSizeX+j)+1] = 
                                                  (unsigned char)((*it).GetGreen()*255);
              m_DisplaySlice[4*(i*m_SliceSizeX+j)+2] = 
                                                  (unsigned char)((*it).GetBlue()*255);
              m_DisplaySlice[4*(i*m_SliceSizeX+j)+3] = 
                                                 (unsigned char)(cOverlayOpacity*255);  
           }
     
                          break;
        default:
             break;
      }
      
     
      it++;
    }
  }

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
  glDisable(GL_BLEND);

}

void
LandmarkGlRenderMethod2D
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}

void
LandmarkGlRenderMethod2D
::Update()
{

}

} // end of namespace sov
