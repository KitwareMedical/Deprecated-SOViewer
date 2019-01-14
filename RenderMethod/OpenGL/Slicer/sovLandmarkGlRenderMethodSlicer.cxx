/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLandmarkGlRenderMethodSlicer.cxx,v $
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
#include "sovLandmarkGlRenderMethodSlicer.h"

#include <itkLandmarkSpatialObject.h>
#include "sovOpenGL.h"

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

namespace sov
{

LandmarkGlRenderMethodSlicer
::LandmarkGlRenderMethodSlicer( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("LandmarkSpatialObject");
  m_DisplaySlice = NULL;
  
  this->m_LastDrawTime = 0;
}

LandmarkGlRenderMethodSlicer
::~LandmarkGlRenderMethodSlicer( void )
{
  if(m_DisplaySlice)
  {
    delete [] m_DisplaySlice;
  }
}

void
LandmarkGlRenderMethodSlicer
::draw( void )
{
  ObjectListType::iterator  currentObject = m_ObjectList.begin();
  ObjectListType::iterator  lastObject = m_ObjectList.end();
  LandmarkType::Pointer     landmark;  
  unsigned int              cWinOrder[3];
  double                    spacing[3];

  //if( this->GetMTime() > this->m_LastDrawTime )
  //{

  for(; currentObject!=lastObject; currentObject++)
    {
    landmark = static_cast<LandmarkType *>((*currentObject)->GetObject());

    switch(m_Orientation)
      {
      case 0 :
        cWinOrder[0] = 1;
        cWinOrder[1] = 2;
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
        break;
      }

    spacing[0] = landmark->GetSpacing()[cWinOrder[0]];
    spacing[1] = landmark->GetSpacing()[cWinOrder[1]];
    spacing[2] = landmark->GetSpacing()[cWinOrder[2]];

    LandmarkType::PointListType::const_iterator it = landmark->GetPoints().begin();
    LandmarkType::PointListType::const_iterator end = landmark->GetPoints().end();
    
    float error;
    float epsilon = spacing[2]/2.0;
    GLfloat sizeX = 5.0/spacing[0];
    GLfloat sizeY = 5.0/spacing[1];

    while(it != end)
      { 
      error = fabs((*it).GetPosition()[cWinOrder[2]]-(m_SliceNumber[this->m_Orientation]*spacing[2]));
      
      if( error < epsilon )
        {
        GLfloat i = (this->m_FlipX==0?(*it).GetPosition()[cWinOrder[0]]/spacing[0]:this->m_SliceSizeX-((*it).GetPosition()[cWinOrder[0]]/spacing[0]));
        GLfloat j = (this->m_FlipY==0?(*it).GetPosition()[cWinOrder[1]]/spacing[1]:this->m_SliceSizeY-((*it).GetPosition()[cWinOrder[1]]/spacing[1]));
        
        LandmarkPointType::PixelType color = (*it).GetColor();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin(GL_LINES);
          glColor4f(color[0],color[1],color[2],color[3]);
          glVertex2f(i,j-sizeY);
          glVertex2f(i,j+sizeY);
          glVertex2f(i-sizeX,j);
          glVertex2f(i+sizeX,j);
        glEnd();

        glDisable(GL_BLEND);
        }
      it++;
      }
    }
  
  //this->m_LastDrawTime = this->GetMTime();
  //}
}

void
LandmarkGlRenderMethodSlicer
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}

void
LandmarkGlRenderMethodSlicer
::Update()
{
  /*ObjectListType::iterator            currentObject,lastObject;
  currentObject = m_ObjectList->begin();
  lastObject = m_ObjectList->end();
   
  for(; currentObject!=lastObject; currentObject++)
  {
    
   
  }*/
}

} // end of namespace sov
