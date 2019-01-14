/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceTubeGlRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovSurfaceTubeGlRenderMethod3D.h"

namespace sov
{

SurfaceTubeGlRenderMethod3D
::SurfaceTubeGlRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("TubeSpatialObject");
  m_DisplayList = 1;
}

SurfaceTubeGlRenderMethod3D
::~SurfaceTubeGlRenderMethod3D()
{
}

/** Draw the surface of each object attached to this
 *  render method */
void 
SurfaceTubeGlRenderMethod3D
::draw( void )
{
  if( m_DisplayList != 0 )
  {
    BuildDisplayList(); 
  }
}

void
SurfaceTubeGlRenderMethod3D
::BuildDisplayList()
{

  unsigned int tubeIndex = 0;

    /*if( m_DisplayList != -1 )
      {
      glDeleteLists(m_DisplayList,1);
      }
    */
   
  /*if(m_DisplayList != -1)
  {*/
    //m_DisplayList = 2;//glGenLists(1);
   /*m_DisplayList++;
       std::cout << "creating list #" << m_DisplayList << std::endl;
    glNewList(m_DisplayList,GL_COMPILE);  
   */     
    // setting environment for blending...
    /*glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glDisable(GL_DEPTH_TEST);*/

    ObjectListType::iterator            currentObject;
    currentObject = m_ObjectList->begin();

    for(; tubeIndex<m_NumberOfTubes; tubeIndex++,currentObject++)
    {
    
    
     float* m_MaterialDiffuse = new float[4];
    m_MaterialDiffuse[0]=(*currentObject)->GetProperty()->GetRed();
    m_MaterialDiffuse[1]=(*currentObject)->GetProperty()->GetGreen();
    m_MaterialDiffuse[2]=(*currentObject)->GetProperty()->GetBlue();
    m_MaterialDiffuse[3]=1;

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_MaterialDiffuse);

      //glColor3fv(m_Color[tubeIndex]);
      //glColor4f(m_Color[tubeIndex][0],m_Color[tubeIndex][1],m_Color[tubeIndex][2],0.6);
      gleSetNumSides(20);
      glePolyCone ( m_Size[tubeIndex]-1,
                    m_Point[tubeIndex],         // polyline vertices
                    m_Color[tubeIndex],         //color_array, colors at polyline verts
                    m_Radius[tubeIndex]);       // cone radii at polyline verts 

     //
    }

    //glEndList();
  //}
}

/** Rebuild the display list using the object attached 
 *  to this render method. */
void
SurfaceTubeGlRenderMethod3D
::Update()
{

  ObjectListType::iterator            currentObject,lastObject;
  TubePointListPointer                tubePointList;
  TubePointListType::iterator         currentTubePoint,lastTubePoint,test;
  TubeListType                        tubeList;
  TubeListType::iterator              currentTube,lastTube;
  PointPointer                        centerLinePoint;
 
  currentObject = m_ObjectList->begin();
  lastObject = m_ObjectList->end();

  unsigned int tubeIndex=0;
  unsigned int pointIndex=0;

  for(; currentObject!=lastObject; currentObject++)
  {
    tubeList = dynamic_cast<TubeType *>((*currentObject))->GetChildren();
    currentTube = tubeList.begin();
    lastTube = tubeList.end();

    for(; currentTube!=lastTube; currentTube++,tubeIndex++)
    {
      pointIndex=0;

      tubePointList = dynamic_cast<TubeType *>((*currentTube))->GetPoints();
      currentTubePoint = tubePointList->begin();
      lastTubePoint = tubePointList->end();
 
      for(; currentTubePoint!=lastTubePoint; currentTubePoint++,pointIndex++)
      {
        centerLinePoint = &(*currentTubePoint)->GetPosition();
        m_Point[tubeIndex][pointIndex][0] = (*centerLinePoint)[0];
        m_Point[tubeIndex][pointIndex][1] = (*centerLinePoint)[1];
        m_Point[tubeIndex][pointIndex][2] = (*centerLinePoint)[2];
        m_Radius[tubeIndex][pointIndex] = (*currentTubePoint)->GetRadius();
        m_Color[tubeIndex][pointIndex][0]=(*currentTubePoint)->GetColor().GetRed();
        m_Color[tubeIndex][pointIndex][1]=(*currentTubePoint)->GetColor().GetGreen();
        m_Color[tubeIndex][pointIndex][2]=(*currentTubePoint)->GetColor().GetBlue();
      }
      m_Size[tubeIndex]=pointIndex;
    }
  }
  m_NumberOfTubes=tubeIndex;
  
  BuildDisplayList();
}

/** Print the object and its properties in a stream */
void
SurfaceTubeGlRenderMethod3D
::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
{

}

} // end of namespace sov
