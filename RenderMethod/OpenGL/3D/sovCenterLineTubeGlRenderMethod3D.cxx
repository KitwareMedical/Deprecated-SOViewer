/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovCenterLineTubeGlRenderMethod3D.cxx,v $
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
#include "sovCenterLineTubeGlRenderMethod3D.h"
#include "sovOpenGL.h"

namespace sov
{

CenterLineTubeGlRenderMethod3D
::CenterLineTubeGlRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("TubeSpatialObject");

  m_PointListList = new PointListListType();
  m_ColorList = new ColorListType();

  m_DisplayList = -1;
  m_Modified = true;
}

CenterLineTubeGlRenderMethod3D
::~CenterLineTubeGlRenderMethod3D()
{
}

/**************************************************
 *
 * Draw the center line of each object attached to this
 * render method...
 *
 **************************************************/
void 
CenterLineTubeGlRenderMethod3D
::draw( void )
{
  //CheckDisplayList();
  BuildDisplayList();
  //glCallList(m_DisplayList);
}

void
CenterLineTubeGlRenderMethod3D
::CheckDisplayList()
{
  if( m_DisplayList == -1  )
    {
    m_DisplayList = glGenLists(1);
    m_Modified = true;
    }

  if( m_Modified )
  {
    glNewList(m_DisplayList,GL_COMPILE);
    BuildDisplayList();
    glEndList();
    m_Modified = false;
  }
}

void
CenterLineTubeGlRenderMethod3D
::BuildDisplayList()
{

  PointListListType::iterator   currentList,lastList;
  PointListType *               list;
  PointListType::iterator       currentPoint,lastPoint;
  PointType *                   point;
  ColorListType::iterator       currentColor;

  currentList = m_PointListList->begin();
  currentColor = m_ColorList->begin();
  lastList = m_PointListList->end();
/*
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);
  glDisable(GL_BLEND);
  glDisable(GL_LIGHTING);
*/

  glEnable(GL_COLOR_MATERIAL); 

  for(; currentList != lastList; currentList++,currentColor++ )
  {
    list = (*currentList);
    currentPoint = list->begin();
    lastPoint = list->end();
    
    
    glBegin(GL_LINE_STRIP);

    float* m_MaterialDiffuse = new float[4];
    m_MaterialDiffuse[0]=0;//(*currentObject)->GetProperty()->GetRed();
    m_MaterialDiffuse[1]=1;//(*currentObject)->GetProperty()->GetGreen();
    m_MaterialDiffuse[2]=0;//(*currentObject)->GetProperty()->GetBlue();
    m_MaterialDiffuse[3]=1;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,m_MaterialDiffuse);
    //glColor4fv((*currentColor));
    for(; currentPoint != lastPoint; currentPoint++ )
    {
      point = (*currentPoint);
      glVertex3f((*point)[0],(*point)[1],(*point)[2]);
    }
    glEnd();
    
  }
  
  glDisable(GL_COLOR_MATERIAL); 

}

/***************************************************
 *
 * Rebuild the display list using the object attached 
 * to this render method.
 *
 ***************************************************/

void
CenterLineTubeGlRenderMethod3D
::Update()
{

  ObjectListType::iterator            currentObject,lastObject;
  TubePointListType                   tubePointList;
  TubePointListType::iterator         currentTubePoint,lastTubePoint,test;
  TubeListType *                      tubeList;
  TubeListType::iterator              currentTube,lastTube;
  PointType                           point;
  PointListType *                     pointList;
  PixelType                           pixel;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  m_PointListList->clear();
  m_ColorList->clear();

  for(unsigned int objectIndex=1; currentObject!=lastObject; currentObject++,objectIndex++ )
  {
    tubeList = dynamic_cast<TubeType *>((*currentObject)->GetObject())->GetChildren();
    currentTube = tubeList->begin();
    lastTube = tubeList->end();


    for(unsigned int tubeIndex=1; currentTube!=lastTube; currentTube++,tubeIndex++)
      {
      tubePointList = dynamic_cast<TubeType *>((*currentTube))->GetPoints();
      currentTubePoint = tubePointList.begin();
      lastTubePoint = tubePointList.end();

      pointList = new PointListType();
      pointList->clear();

      for(unsigned int pointIndex=1; currentTubePoint!=lastTubePoint; currentTubePoint++, pointIndex++ )
      {
        //point = &(*currentTubePoint).GetPosition();
        point = (*currentTubePoint).GetPosition();
        PointPointer newPoint = new PointType();
        (*newPoint)[0] = point[0];
        (*newPoint)[1] = point[1];
        (*newPoint)[2] = point[2];
        pointList->push_back(newPoint);
      }
      
      pixel = (*currentTube)->GetProperty()->GetColor();
      
      ColorType * color = new ColorType(4);
      color[0] = pixel.GetRed();
      color[1] = pixel.GetGreen();
      color[2] = pixel.GetBlue();
      color[3] = 1;//pixel.GetAlpha();

      m_PointListList->push_back(pointList);
      m_ColorList->push_back(color);
      }
    }

  m_Modified = true;
}

/****************************************************
 *
 * Print the object and its properties in a stream...
 *
 ****************************************************/

void
CenterLineTubeGlRenderMethod3D
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}

} // end of namespace sov
