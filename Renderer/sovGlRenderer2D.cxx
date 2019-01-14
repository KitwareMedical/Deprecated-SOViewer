/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderer2D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:58 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovGlRenderer2D.h"
#include "itkImageSpatialObject.h"

namespace sov
{

/** Constructor */
GlRenderer2D
::GlRenderer2D()
{   
  m_Type = "OpenGL";
  m_DisplayVolume = NULL;
  
  m_SliceSizeX = 0;
  m_SliceSizeY = 0;
  m_SliceSpacing.Fill(1);
}

/** Destructor */
GlRenderer2D
::~GlRenderer2D()
{

}

/** Print the object */
void GlRenderer2D
::PrintSelf (std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

/** Add render method */
void GlRenderer2D
::AddRenderMethod( SuperclassMethodType * method)
{
  Superclass::AddRenderMethod(method);
}

/** Set the image size in the case of no image is present */
void GlRenderer2D
::SetImageSize(unsigned int x, unsigned int y)
{
  m_SliceSizeX = x;
  m_SliceSizeY = y;

  MethodListType::iterator it,end;
 
  it = m_MethodList.begin();
  end = m_MethodList.end();

  /** Reallocate the image volume */
  if(m_DisplayVolume)
  {
    delete [] m_DisplayVolume;
  }
  
  m_DisplayVolume = new unsigned char [x*y*4];
  memset(m_DisplayVolume, 0, x*y*4);

  for(; it != end; it++ )
  {
    dynamic_cast<MethodType*>((*it).GetPointer())->SetImageSize(x,y);
    dynamic_cast<MethodType*>((*it).GetPointer())->SetDisplayVolume(m_DisplayVolume);
  }

}

/** Add render method */
void GlRenderer2D
::draw(void)
{
  if( m_Activated )
  {
    itk::Vector<double,2> m_SliceSize;  
    m_SliceSize[0] = m_SliceSizeX;
    m_SliceSize[1] = m_SliceSizeY;
 
    itk::Vector<double,2> m_ViewPosition;   
      // Compute slice size in spatial coordinates
    itk::Vector<double,2> worldSize;

    worldSize[0] = m_SliceSize[0] * m_SliceSpacing[0];
    worldSize[1] = m_SliceSize[1] * m_SliceSpacing[1];

    // Set the view position (position of the center of the image?)
    m_ViewPosition = worldSize * 0.5f;

    // Reduce the width and height of the slice by the margin
    int margin = 0; 
    vnl_vector_fixed<int,2> szCanvas = vnl_vector_fixed<int,2>((int)(m_Width),(int)(m_Height)) - vnl_vector_fixed<int,2>(2 * margin);
  
    // Compute the ratios of window size to slice size
    vnl_vector_fixed<float,2> ratios(szCanvas(0) / worldSize[0],szCanvas(1) / worldSize[1]);

    // The zoom factor is the bigger of these ratios, the number of pixels 
    // on the screen per millimeter in world space
    double m_ViewZoom = ratios.min_value();
    glScalef(m_ViewZoom,m_ViewZoom,1.0);

    glTranslated(-m_ViewPosition[0],-m_ViewPosition[1],0.0);

    glScalef(m_SliceSpacing[0],m_SliceSpacing[1],1.0);

    MethodListType::iterator it,end;
    it = m_MethodList.begin();
    for(; it != m_MethodList.end(); it++ )
    { 
      if((*it)->IsSupported("ImageSpatialObject"))
      { 
        (*it)->draw();
      }
    }

    it = m_MethodList.begin();
    for(; it != m_MethodList.end(); it++ )
    { 
      if(!(*it)->IsSupported("ImageSpatialObject"))
      { 
        (*it)->draw();
      }
    }
  }
}


/** Add object */
void GlRenderer2D
::AddObject( ObjectType * object, bool addChildren)
{
  
  Superclass::AddObject(object,addChildren);

  if(!strcmp(object->GetTypeName(),"ImageSpatialObject"))
  {     
    SetImageSize((int)(dynamic_cast<itk::SpatialObject<2>*>(object)->GetBoundingBox()->GetBounds()[1]),
                 (int)(dynamic_cast<itk::SpatialObject<2>*>(object)->GetBoundingBox()->GetBounds()[3])
                );
  }
  else // we need to set the image size 
    {
    MethodListType::iterator it = m_MethodList.begin();

    for(; it != m_MethodList.end(); it++ )
      {
        dynamic_cast<MethodType*>((*it).GetPointer())->SetImageSize(m_SliceSizeX,m_SliceSizeY);
        dynamic_cast<MethodType*>((*it).GetPointer())->SetDisplayVolume(m_DisplayVolume);
      }
    }
  
}

/** Add Object */
void GlRenderer2D
::AddObject( ObjectType * object, SuperclassMethodType * method)
{
  Superclass::AddObject(object, method);
}

/** Set the scene */
void GlRenderer2D
::SetScene( SceneType * scene )
{
  m_Scene = scene;
  SceneType::ObjectListType * list = m_Scene->GetObjects();
  SceneType::ObjectListType::iterator it = list->begin();
  SceneType::ObjectListType::iterator itEnd = list->end();
  while(it != itEnd)
  {
    AddObject(*it);
    it++;
  }

  delete list;

  m_NumberOfObjects = m_Scene->GetNumberOfObjects();
}

} //end of namespace sov
