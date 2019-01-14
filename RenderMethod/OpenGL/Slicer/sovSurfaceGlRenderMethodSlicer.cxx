/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceGlRenderMethodSlicer.cxx,v $
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
#include "sovSurfaceGlRenderMethodSlicer.h"

#include <itkSurfaceSpatialObject.h>
#include "sovOpenGL.h"

namespace sov
{

SurfaceGlRenderMethodSlicer
::SurfaceGlRenderMethodSlicer( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("SurfaceSpatialObject");
  m_DisplaySlice = NULL;
}

SurfaceGlRenderMethodSlicer
::~SurfaceGlRenderMethodSlicer( void )
{
  if(m_DisplaySlice)
  {
    delete [] m_DisplaySlice;
  }

}

void
SurfaceGlRenderMethodSlicer
::draw( void )
{
  ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
   
  for(; currentObject!=lastObject; currentObject++)
  {
  
    /** Reallocate the image volume */
    if(!m_DisplaySlice)
    {
      //delete [] m_DisplaySlice;
      m_DisplaySlice = new unsigned char [m_SliceSizeX*m_SliceSizeY*4];
    }


    //if(m_Orientation)
   // {
        typedef SurfaceSpatialObject<3> SurfaceType;
    //std::cout << "SurfaceGlRenderMethodSlicer::drawing " << std::endl;
    //std::cout << "Size = " <<dynamic_cast<SurfaceType*>((*currentObject)->GetObject())->GetPoints().size() << std::endl;

      memset(m_DisplaySlice, 0, m_SliceSizeX*m_SliceSizeY*4);

  

      unsigned int cWinOrder[3];
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
  

      SurfaceType::PointListType::const_iterator it = dynamic_cast<SurfaceType*>((*currentObject)->GetObject())->GetPoints().begin();

      while(it != dynamic_cast<SurfaceType*>((*currentObject)->GetObject())->GetPoints().end())
      {
        if( (unsigned int) ( (*it).GetPosition()[cWinOrder[2]] ) == m_SliceNumber[cWinOrder[2]] )
        {
          unsigned int i = (unsigned int)(*it).GetPosition()[cWinOrder[1]];
          unsigned int j = (unsigned int)(*it).GetPosition()[cWinOrder[0]];
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
SurfaceGlRenderMethodSlicer
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}

void
SurfaceGlRenderMethodSlicer
::Update()
{

  this->draw();
  /*std::cout << "SurfaceGlRenderMethodSlicer::Updating " << std::endl;
  ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList->begin();
  lastObject = m_ObjectList->end();
   
  for(; currentObject!=lastObject; currentObject++)
  {
    
   
  }*/
}

} // end of namespace sov
