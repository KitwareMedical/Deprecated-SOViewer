/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLineGlRenderMethod2D.cxx,v $
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
#include "sovLineGlRenderMethod2D.h"

#include <itkLineSpatialObject.h>
#include "sovOpenGL.h"

namespace sov
{

LineGlRenderMethod2D
::LineGlRenderMethod2D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("LineSpatialObject");
  m_DisplaySlice = NULL;
}

LineGlRenderMethod2D
::~LineGlRenderMethod2D( void )
{
  if(m_DisplaySlice)
  {
    delete [] m_DisplaySlice;
  }
}

void
LineGlRenderMethod2D
::draw( void )
{
  ObjectListType::iterator            currentObject,lastObject;
 
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
    LineType::Pointer Line = static_cast<LineType *>((*currentObject)->GetObject());

    double cOverlayOpacity = 0.7;
       
    LineType::PointListType::const_iterator it = Line->GetPoints().begin();

    unsigned int old_x = 0;
    unsigned int old_y= 0;

    while(it != Line->GetPoints().end())
    {
      unsigned int x = (unsigned int)(*it).GetPosition()[0];
      unsigned int y = (unsigned int)(*it).GetPosition()[1];

      if (( ((unsigned int)(x) != old_x) || ((unsigned int)(y) != old_y) )
        && ( ((unsigned int)(x)>0) && ((unsigned int)(x)<m_SliceSizeX) 
        && ((unsigned int)(y)>0) && ((unsigned int)(y)<m_SliceSizeY) )
         )
      {
        old_x = x;
        old_y = y;
  
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
LineGlRenderMethod2D
::PrintSelf (std::ostream & itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

void
LineGlRenderMethod2D
::Update()
{

}

} // end of namespace sov
