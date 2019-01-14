/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovTubeGlRenderMethod2D.cxx,v $
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
#include "sovTubeGlRenderMethod2D.h"

#include <itkTubeSpatialObject.h>
#include "sovOpenGL.h"

namespace sov
{

TubeGlRenderMethod2D
::TubeGlRenderMethod2D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("TubeSpatialObject");
  m_DisplaySlice = NULL;
  m_ShowRadius = true;
}

TubeGlRenderMethod2D
::~TubeGlRenderMethod2D( void )
{
  if(m_DisplaySlice)
  {
    delete [] m_DisplaySlice;
  }
}

void
TubeGlRenderMethod2D
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
    TubeType::Pointer Tube = static_cast<TubeType *>((*currentObject)->GetObject());

    double cOverlayOpacity = 0.7;
       
    TubeType::PointListType::const_iterator it = Tube->GetPoints().begin();
    TubeType::PointListType::const_iterator next = Tube->GetPoints().begin();
    
    double x,y;
    unsigned int old_x = 0;
    unsigned int old_y = 0;
    double prevx = 0;
    double prevy = 0;

    next++;

    while(next != Tube->GetPoints().end())
    {
      unsigned int currentx = (unsigned int)(*it).GetPosition()[0];
      unsigned int currenty = (unsigned int)(*it).GetPosition()[1];

      double radius = (*it).GetRadius();
      double radiusnext = (*next).GetRadius();
      unsigned int nextx = (unsigned int)(*next).GetPosition()[0];
      unsigned int nexty = (unsigned int)(*next).GetPosition()[1];

      // do the interpolation beetween two points in the center line
      double lenght = sqrt((double)(nextx-currentx)*(nextx-currentx)+ (nexty-currenty)*(nexty-currenty));
      for(double l=0;l<1;l+=1/lenght) 
      {
        x= currentx+l*((double)nextx-(double)currentx);
        y= currenty+l*((double)nexty-(double)currenty);

        if (( ((unsigned int)(x) != old_x) || ((unsigned int)(y) != old_y) )
          && ( ((unsigned int)(x)>0) && ((unsigned int)(x)<m_SliceSizeX) 
          && ((unsigned int)(y)>0) && ((unsigned int)(y)<m_SliceSizeY) ))
        {
          old_x = (unsigned int)x;
          old_y = (unsigned int)y;
  
          unsigned int i = (unsigned int)y;
          unsigned int j = (unsigned int)x;
          if(l==0)
          {
            m_DisplaySlice[4*(i*m_SliceSizeX+j)] = 
                          (unsigned char)((*it).GetRed()*255);
            m_DisplaySlice[4*(i*m_SliceSizeX+j)+1] = 
                                     (unsigned char)((*it).GetGreen()*255);
            m_DisplaySlice[4*(i*m_SliceSizeX+j)+2] = 
                                    (unsigned char)((*it).GetBlue()*255);
            m_DisplaySlice[4*(i*m_SliceSizeX+j)+3] = 
                                   (unsigned char)(cOverlayOpacity*255);
          }
          else
          {
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

      if(m_ShowRadius)
      {

        // do the same for the radius by taking the normal
        // if no normal computed, we compute it using finite differences
        CovariantVector<double,2> normal =  (*it).GetNormal1();  
      
        // Computes the normals if they do not exist
        if((normal[0] == 0) && (normal[1] == 0))
        {
          double tangentx,tangenty;
          if(it == Tube->GetPoints().begin())
          {
            tangentx = ((double)nextx-(double)currentx)/2;
            tangenty = ((double)nexty-(double)currenty)/2;
          }
          else
          {
            tangentx = ((double)nextx-(prevx))/2;
            tangenty = ((double)nexty-(prevy))/2;
          }

          normal[0] = tangenty; 
          normal[1] = -tangentx;
        }

        
        normal.Get_vnl_vector().normalize();

        for(double l=0;l<1;l+=1/lenght) 
        {
          double posx= currentx+l*((double)nextx-(double)currentx);
          double posy= currenty+l*((double)nexty-(double)currenty);


          x= posx+(radius+l*(radiusnext-radius))*normal[0];
          y= posy+(radius+l*(radiusnext-radius))*normal[1];

          if (( ((unsigned int)(x) != old_x) || ((unsigned int)(y) != old_y) )
            && ( ((unsigned int)(x)>0) && ((unsigned int)(x)<m_SliceSizeX) 
            && ((unsigned int)(y)>0) && ((unsigned int)(y)<m_SliceSizeY) ))
          {
            old_x = (unsigned int)x;
            old_y = (unsigned int)y;
  
            unsigned int i = (unsigned int)y;
            unsigned int j = (unsigned int)x;

            m_DisplaySlice[4*(i*m_SliceSizeX+j)] = 
                          (unsigned char)((*it).GetRed()*255);
            m_DisplaySlice[4*(i*m_SliceSizeX+j)+1] = 
                                    (unsigned char)((*it).GetGreen()*255);
            m_DisplaySlice[4*(i*m_SliceSizeX+j)+2] = 
                                    (unsigned char)((*it).GetBlue()*255);
            m_DisplaySlice[4*(i*m_SliceSizeX+j)+3] = 
                                   (unsigned char)(cOverlayOpacity*255);     
          }

          x= posx-(radius+l*(radiusnext-radius))*normal[0];
          y= posy-(radius+l*(radiusnext-radius))*normal[1];

          if (( ((unsigned int)(x) != old_x) || ((unsigned int)(y) != old_y) )
            && ( ((unsigned int)(x)>0) && ((unsigned int)(x)<m_SliceSizeX) 
            && ((unsigned int)(y)>0) && ((unsigned int)(y)<m_SliceSizeY) ))
          {
            old_x = (unsigned int)x;
            old_y = (unsigned int)y;
  
            unsigned int i = (unsigned int)y;
            unsigned int j = (unsigned int)x;

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

      prevx = currentx,
      prevy = currenty;
      
      it++;
      next++;
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
TubeGlRenderMethod2D
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}

void
TubeGlRenderMethod2D
::Update()
{

}

} // end of namespace sov
