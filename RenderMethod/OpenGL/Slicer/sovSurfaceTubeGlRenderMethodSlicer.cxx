/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceTubeGlRenderMethodSlicer.cxx,v $
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
#include "sovSurfaceTubeGlRenderMethodSlicer.h"

#include <itkTubeSpatialObject.h>
#include "sovOpenGL.h"

namespace sov
{

SurfaceTubeGlRenderMethodSlicer
::SurfaceTubeGlRenderMethodSlicer( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("TubeSpatialObject");
  m_DisplaySlice = NULL;
}

SurfaceTubeGlRenderMethodSlicer
::~SurfaceTubeGlRenderMethodSlicer( void )
{
  if(m_DisplaySlice)
  {
    delete [] m_DisplaySlice;
  }
}

void
SurfaceTubeGlRenderMethodSlicer
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
  unsigned int cWinOrder[3];

  for(; currentObject!=lastObject; currentObject++)
  {
     //tubeList = dynamic_cast<TubeType *>((*currentObject)->GetObject());
    //currentTube = tubeList.begin();
    //lastTube = tubeList.end();

    
    //for(tubeList.begin(); currentTube!=lastTube; currentTube++)
   // {
      TubeType::Pointer tube = static_cast<TubeType *>((*currentObject)->GetObject());
      
      switch(m_Orientation)
      {
        case 0 :
          cWinOrder[0] = 2;
          cWinOrder[1] = 1;
          cWinOrder[2] = 0;
        break;
        case 1 :
          cWinOrder[0] = 0;
          cWinOrder[1] = 2;
          cWinOrder[2] = 1;
        break;
        default:
        case 2 :
          m_Orientation = 2;
          cWinOrder[0] = 0;
          cWinOrder[1] = 1;
          cWinOrder[2] = 2;
      }

      double cOverlayOpacity = 0.7;
       
      TubeType::PointListType::const_iterator it = tube->GetPoints().begin();

      unsigned int old_cx = 0;
      unsigned int old_cy= 0;
      unsigned int old_cz = 0;

      while(it != tube->GetPoints().end())
      {
        
        double cx = (*it).GetPosition()[cWinOrder[0]];
        double cy = (*it).GetPosition()[cWinOrder[1]];
        double cz = (*it).GetPosition()[cWinOrder[2]];

        if ( ((unsigned int)(cx) != old_cx) || ((unsigned int)(cy) != old_cy) || ((unsigned int)(cz) != old_cz) )
        {
          old_cx = (unsigned int)cx;
          old_cy = (unsigned int)cy;
          old_cz = (unsigned int)cz;

          unsigned int x,y,z;
          unsigned int old_x,old_y;

          old_x = 0;
          old_y = 0;


          for(double r=0;r<=(*it).GetRadius();r+=(*it).GetRadius()/10)
          {
            for(double teta = 0;teta<6.28;teta+=0.1)
            {
              double ct = cos(teta);
              double st = sin(teta);
              
              z=(unsigned int)(cz+r*ct*(*it).GetNormal1()[2]+r*st*(*it).GetNormal2()[2]);
             
              if(z == m_SliceNumber[2])
              {
                x=(unsigned int)(cx+r*ct*(*it).GetNormal1()[0]+r*st*(*it).GetNormal2()[0]);
                y=(unsigned int)(cy+r*ct*(*it).GetNormal1()[1]+r*st*(*it).GetNormal2()[1]);

                if( ((x != old_x) || (y != old_y))
                    && (x<m_SliceSizeX)
                    && (y<m_SliceSizeY)
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
              }
            }
          }
        }
     
        it++;
      }
    
    //}
    
  }


  if(m_ObjectList.size()>0)
  {
    double cWinZoom = 1;
    double m_Spacing[3];
    unsigned int cFlipX[3];
    unsigned int cFlipY[3];
 
    for(unsigned int i=0;i<3;i++)
    {
      m_Spacing[i]=1;
      cFlipX[i]=0;
      cFlipY[i]=0;
    }

    float scale0 = m_Width/(float)m_SliceSizeX * cWinZoom
      * fabs(m_Spacing[cWinOrder[0]])/fabs(m_Spacing[0]);
    float scale1 = m_Height/(float)m_SliceSizeY * cWinZoom
      * fabs(m_Spacing[cWinOrder[1]])/fabs(m_Spacing[0]);
 
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

}

void
SurfaceTubeGlRenderMethodSlicer
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}

void
SurfaceTubeGlRenderMethodSlicer
::Update()
{

}

} // end of namespace sov
