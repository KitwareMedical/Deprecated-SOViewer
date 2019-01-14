/*=========================================================================

Program:   itkUNC
Module:    $RCSfile: sovGlRendererSlicer.cxx,v $
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
#include "sovGlRendererSlicer.h"
#include "itkImageSpatialObject.h"
#include "sovOpenGL.h"

namespace sov
{

/** Constructor */
GlRendererSlicer
::GlRendererSlicer()
{   
  m_Type = "OpenGL";
  m_DisplayVolume = NULL;
  
  m_ViewZoom = 1.0;
  m_SliceSizeX = 0;
  m_SliceSizeY = 0;
  m_SliceNumber[0] = 0;
  m_SliceNumber[1] = 0;
  m_SliceNumber[2] = 0;
  m_Orientation = 0;
  m_Spacing.Fill(1);

  m_AxisOpacity = 0.7;

  m_AxisColor[0][0] = 1.0;
  m_AxisColor[0][1] = 0.0;
  m_AxisColor[0][2] = 0.0;

  m_AxisColor[1][0] = 0.0;
  m_AxisColor[1][1] = 1.0;
  m_AxisColor[1][2] = 0.0;

  m_AxisColor[2][0] = 0.0;
  m_AxisColor[2][1] = 0.0;
  m_AxisColor[2][2] = 1.0;

  this->m_ClickIndex.Fill(0);
  this->m_ClickPosition.Fill(0);

  m_ViewCrosshair = false;
}

/** Destructor */
GlRendererSlicer
::~GlRendererSlicer()
{

}

/** Print the object */
void GlRendererSlicer
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
  //Superclass::PrintSelf(os,indent);
}

/** Add render method */
void GlRendererSlicer
::AddRenderMethod( SuperclassMethodType * method)
{
  //static_cast<MethodType*>(method)->SetSliceNumber(m_SliceNumber);
  static_cast<MethodType*>(method)->SetOrientation(m_Orientation);
  Superclass::AddRenderMethod(method);
}



/** Set the slice to be displayed */
void GlRendererSlicer
::SetSliceNumber(unsigned int slice)
{
  VisualObjectListType::iterator it1 = m_VisualObjectList.begin();
  while(it1 != m_VisualObjectList.end())
    {
    if((*it1)->GetObject())
      {
      if(!strcmp((*it1)->GetObject()->GetTypeName(),"ImageSpatialObject"))
        {
        //this has been commented out since all images displayed within this renderer 
        //are expected to have the same orientation (refer to SetOrientation())
        //unsigned int orientation = static_cast<VisualImageProperty*>((*it1)->GetProperty())->GetOrientation();
               
        //if there is at least one image for which it makes sense to increment the slice position
        //the slice will be incremented (m_SliceNumber)
        if(slice<static_cast<itk::ImageSpatialObject<3>*>((*it1)->GetObject())->GetImage()->GetLargestPossibleRegion().GetSize()[this->m_Orientation])
          {
          static_cast<itk::ImageSpatialObject<3>*>((*it1)->GetObject())->SetSlicePosition(this->m_Orientation,slice);
          this->m_SliceNumber[this->m_Orientation] = slice;
          }

        //if there is one image for which it does not make sense to increment the slice position
        //the slice number will not be incremented (m_SliceNumber)
        //note: this approach does not work since the SetSliceNumber() for the render method do not
        //      modify the position of the slice displayed. 
        //if(slice>=static_cast<itk::ImageSpatialObject<3>*>((*it1)->GetObject())->GetImage()->GetLargestPossibleRegion().GetSize()[orientation])
          //{
          //return;
          //}
        }
      }
    it1++;
    }

  MethodListType::iterator it,end;
 
  it = m_MethodList.begin();
  end = m_MethodList.end();
  for(; it != end; it++ )
    {
    MethodType * method = dynamic_cast<MethodType*>((*it).GetPointer());
    if( method )
      {
      method->SetSliceNumber(this->m_SliceNumber[this->m_Orientation],this->m_Orientation);
      }
    }
}

/** Set the image size in the case that no image is present */
void GlRendererSlicer
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
    dynamic_cast<MethodType*>((*it).GetPointer())->
      SetDisplayVolume(m_DisplayVolume);
  }


}


// Set the orientation of the object to be viewed
void GlRendererSlicer
::SetOrientation(ObjectType * object, unsigned int orientation)
  {
  VisualObjectListType::iterator it = m_VisualObjectList.begin();
  while(it != m_VisualObjectList.end())
    {
    if((*it)->GetObject()  == object)
      {
      if(!strcmp(object->GetTypeName(),"ImageSpatialObject"))
        {
        static_cast<VisualImageProperty*>((*it)->GetProperty())->SetOrientation(orientation);
        }
      else
        {
        std::cout << "SetOrientation:: Only ImageSpatialObject can be used for the moment" << std::endl;
        }
      }
    it++;
    }

  }
/** Set the orientation of the slice to be viewed 
 *  The Scene should be set as well as the image for this function to work
 *
 */
void GlRendererSlicer
::SetOrientation(unsigned int orientation)
{
  m_Orientation = orientation;
  
  MethodListType::iterator it, end;
  it = m_MethodList.begin();
  end = m_MethodList.end();
  for(; it != end; it++ )
  {
    dynamic_cast<MethodType*>((*it).GetPointer())->
      SetOrientation(orientation);
  }

  VisualObjectListType::iterator it2 = m_VisualObjectList.begin();

  while(it2 != m_VisualObjectList.end())
    {
    if(!strcmp((*it2)->GetObject()->GetTypeName(),"ImageSpatialObject"))
      {
      static_cast<VisualImageProperty*>((*it2)->GetProperty())->SetOrientation(this->m_Orientation);     
      static_cast<itk::ImageSpatialObject<3>*>((*it2)->GetObject())->SetSlicePosition(this->m_Orientation,this->m_SliceNumber[this->m_Orientation]);
      }
    it2++;
    }

  AutoSetImageSize(this->m_Orientation);

}

/** Add render method */
void GlRendererSlicer
::draw(void)
{
  if( m_Activated )
  {
    itk::Vector<double,2> sliceSize;  
    sliceSize[0] = m_SliceSizeX;
    sliceSize[1] = m_SliceSizeY;

    itk::Vector<double,2> viewPosition;   
      // Compute slice size in spatial coordinates
    itk::Vector<double,2> worldSize;

    unsigned int widthAxis;
    unsigned int heightAxis;
    unsigned int depthAxis;

    switch(this->m_Orientation)
      {
      case 0:
        widthAxis = 1;
        heightAxis = 2;
        depthAxis = 0;
        break;

      case 1:
        widthAxis = 0;
        heightAxis = 2;
        depthAxis = 1;
        break;

      case 2:
        widthAxis = 0;
        heightAxis = 1;
        depthAxis = 2;
        break;
      }

    //m_Spacing.Fill(1);
    worldSize[0] = sliceSize[0] * m_Spacing[widthAxis];
    worldSize[1] = sliceSize[1] * m_Spacing[heightAxis];

    // Set the view position (position of the center of the image?)
    viewPosition = worldSize * 0.5f;

    // Reduce the width and height of the slice by the margin
    int margin = 0; 
    vnl_vector_fixed<int,2> szCanvas = vnl_vector_fixed<int,2>((int)(m_Width),(int)(m_Height)) - vnl_vector_fixed<int,2>(2 * margin);
  
    // Compute the ratios of window size to slice size
    vnl_vector_fixed<float,2> ratios(szCanvas(0) / worldSize[0],szCanvas(1) / worldSize[1]);

    // Compute the ratios of window size to slice size
    //vnl_vector_fixed<float,2> ratios((float)m_Width/ (float)worldSize[0],(float)m_Height / (float)worldSize[1]);


    // The zoom factor is the bigger of these ratios, the number of pixels 
    // on the screen per millimeter in world space
    m_ViewZoom = ratios.min_value();

    glScalef(m_ViewZoom,m_ViewZoom,1.0);

    glTranslated(-viewPosition[0],-viewPosition[1],0.0);

    glScalef(m_Spacing[widthAxis],m_Spacing[heightAxis],1.0);

    itk::Vector<unsigned int,2> offset;

    MethodListType::iterator it = m_MethodList.begin();
    MethodListType::iterator end = m_MethodList.end();
    MethodType * method;

    for(; it != end; it++ )
      { 
      if((*it)->IsSupported("ImageSpatialObject"))
        { 
        method = dynamic_cast<MethodType*>((*it).GetPointer());
        
        if( method ) 
          {
          method->FlipX(this->m_FlipX);
          method->FlipY(this->m_FlipY);
          }

        (*it)->draw();
        }
      }

    it = m_MethodList.begin();

    for(; it != end; it++ )
    { 
      if(!(*it)->IsSupported("ImageSpatialObject"))
      { 
        method = dynamic_cast<MethodType*>((*it).GetPointer());
        
        if( method ) 
          {
          method->FlipX(this->m_FlipX);
          method->FlipY(this->m_FlipY);
          }

        (*it)->draw();
      }
    }

    if(m_ViewCrosshair)
      {
      //drawing line representing the edges of the image
      glEnable(GL_LINE_STIPPLE);     
      glLineStipple(2,0xaaaa);

      glBegin(GL_LINE_STRIP);
        glColor4f(this->m_AxisColor[depthAxis][0],
                  this->m_AxisColor[depthAxis][1],
                  this->m_AxisColor[depthAxis][2],
                  this->m_AxisOpacity);
        glVertex2i(0,0);
        glVertex2i(0,(int)(sliceSize[1]));
        glVertex2i((int)(sliceSize[0]),(int)(sliceSize[1]));
        glVertex2i((int)(sliceSize[0]),0);
        glVertex2i(0,0);
      glEnd();

      float clickCoordX = (this->m_FlipX==0?this->m_ClickIndex[widthAxis]:sliceSize[0]-this->m_ClickIndex[widthAxis]);
      float clickCoordY = (this->m_FlipY==0?this->m_ClickIndex[heightAxis]:sliceSize[1]-this->m_ClickIndex[heightAxis]);

      //drawing line representing the point clicked
      glBegin(GL_LINES);
        glColor4f(this->m_AxisColor[widthAxis][0],
                  this->m_AxisColor[widthAxis][1],
                  this->m_AxisColor[widthAxis][2],
                  this->m_AxisOpacity);
        glVertex2f(clickCoordX,0);
        glVertex2f(clickCoordX,this->m_SliceSizeY);
        
        glColor4f(this->m_AxisColor[heightAxis][0],
                  this->m_AxisColor[heightAxis][1],
                  this->m_AxisColor[heightAxis][2],
                  this->m_AxisOpacity);
        glVertex2f(0,clickCoordY);
        glVertex2f(this->m_SliceSizeX,clickCoordY);
      glEnd();

      glDisable(GL_LINE_STIPPLE);

      float error = fabs(this->m_ClickIndex[depthAxis]-this->m_SliceNumber[depthAxis]);

      if( error < 0.5 )
        {
        GLfloat sizeX = 3.0/this->m_Spacing[widthAxis];
        GLfloat sizeY = 3.0/this->m_Spacing[heightAxis];

        glBegin(GL_LINE_STRIP);
          glColor4f(this->m_AxisColor[depthAxis][0],
                    this->m_AxisColor[depthAxis][1],
                    this->m_AxisColor[depthAxis][2],
                    this->m_AxisOpacity);
          glVertex2f(clickCoordX-sizeX,clickCoordY-sizeY);
          glVertex2f(clickCoordX-sizeX,clickCoordY+sizeY);
          glVertex2f(clickCoordX+sizeX,clickCoordY+sizeY);
          glVertex2f(clickCoordX+sizeX,clickCoordY-sizeY);
          glVertex2f(clickCoordX-sizeX,clickCoordY-sizeY);
        glEnd();
        }
      }
  }
}


/** Add object */
void GlRendererSlicer
::AddObject( ObjectType * object, bool addChildren )
{
  Superclass::AddObject(object,addChildren);
/*
  unsigned int iX, iY;
  switch (m_Orientation)
  {
  case 0: 
    iX = 2;
    iY = 4;
    break;
  case 1:
    iX = 0;
    iY = 4;
    break;
  case 2:
    iX = 0;
    iY = 2;
    break;
  }
*/
  this->SetOrientation(m_Orientation);
}

/** Add Object */
void GlRendererSlicer
::AddObject( ObjectType * object, SuperclassMethodType * method)
{
  Superclass::AddObject(object, method);
}

/** Automatically set the image size (width and height in pixels on
 * the screen, stored in m_SliceSize[xy]) based on the image objects
 * associated to the renderer.  If there are no image objects, then
 * the image size is left unchanged.  The image size is made large
 * enough to hold the largest ImageSpatialObject in its list. */
void GlRendererSlicer
::AutoSetImageSize(unsigned int orientation)
{

  // Max Size X and Y
  unsigned int msX = 0;
  unsigned int msY = 0;
/*
  unsigned int iX, iY;
  switch (orientation)
  {
  case 0: 
    iX = 2;
    iY = 4;
    break;
  case 1:
    iX = 0;
    iY = 4;
    break;
  case 2:
    iX = 0;
    iY = 2;
    break;
  }
*/
  VisualObjectListType::iterator it, end;
  it = m_VisualObjectList.begin();
  end = m_VisualObjectList.end();
  for (; it != end; it++)
  {
    ObjectType* object = dynamic_cast<ObjectType*>((*it)->GetObject());
    if(object && !strcmp(object->GetTypeName(),"ImageSpatialObject"))
    {
      /*
      itk::SpatialObject<>::BoundingBoxType::BoundsArrayType bounds = 
        static_cast<itk::SpatialObject<>*>(object)->
        GetBoundingBox()->GetBounds();

      unsigned int sX = (unsigned int)(bounds[iX+1] - bounds[iX]);
      unsigned int sY = (unsigned int)(bounds[iY+1] - bounds[iY]);

      if (sX > msX) { msX = sX; }
      if (sY > msY) { msY = sY; }
      */

      //unsigned int msX = 0;
      //unsigned int sY = 0;

      this->m_Spacing = static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetSpacing();

      switch(orientation)
        {
        case 0:
          msX = static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetLargestPossibleRegion().GetSize()[1];
          msY = static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetLargestPossibleRegion().GetSize()[2];
          //m_Spacing[0] =  
          //  static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetSpacing()[1];
          //m_Spacing[1] =  
          //  static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetSpacing()[2];
          break;
        case 1:
          msX = static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetLargestPossibleRegion().GetSize()[0];
          msY = static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetLargestPossibleRegion().GetSize()[2];
          //m_Spacing[0] =  
          //  static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetSpacing()[0];
          //m_Spacing[1] =  
          //  static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetSpacing()[2];
          break;
        case 2:
          msX = static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetLargestPossibleRegion().GetSize()[0];
          msY = static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetLargestPossibleRegion().GetSize()[1];
          //m_Spacing[0] =  
          //  static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetSpacing()[0];
          //m_Spacing[1] =  
          //  static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetSpacing()[1];
          break;
        }
    }
  }

  if (msX > 0 && msY > 0)
  {
    SetImageSize(msX, msY);
  }

}

/** Set the scene */
void GlRendererSlicer
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

void GlRendererSlicer
::Click( int x, int y)
{
  unsigned int widthAxis;
  unsigned int heightAxis;
  unsigned int depthAxis;

  switch(this->m_Orientation)
    {
    case 0:
      widthAxis = 1;
      heightAxis = 2;
      depthAxis = 0;
      break;

    case 1:
      widthAxis = 0;
      heightAxis = 2;
      depthAxis = 1;
      break;

    case 2:
      widthAxis = 0;
      heightAxis = 1;
      depthAxis = 2;
      break;
    }

  

  double heightOffset;
  heightOffset  = this->m_Height/(this->m_ViewZoom);
  heightOffset -= this->m_SliceSizeY*this->m_Spacing[heightAxis];
  heightOffset *= this->m_ViewZoom;
  heightOffset /= 2.0;

  double widthOffset;
  widthOffset  = this->m_Width/this->m_ViewZoom;
  widthOffset -= this->m_SliceSizeX*this->m_Spacing[widthAxis];
  widthOffset *= this->m_ViewZoom;
  widthOffset /= 2.0;

  int realX = (this->m_FlipX==0?x:this->m_Width-x);
  int realY = (this->m_FlipY==0?y:this->m_Height-y);

  int worldX = (int)(realX-widthOffset);
  int worldY = (int)(m_Height-(realY+heightOffset)); //the vertical axis of
                                          // openGl viewport 
                                          //and the interface coordinate system
                                          //are inverted along y

  this->m_ClickIndex[depthAxis] = this->m_SliceNumber[depthAxis];
  this->m_ClickIndex[widthAxis] = (worldX/(m_ViewZoom*m_Spacing[widthAxis]));
  this->m_ClickIndex[heightAxis] = (worldY/(m_ViewZoom*m_Spacing[heightAxis]));

  if( this->m_ClickIndex[widthAxis] > this->m_SliceSizeX ) this->m_ClickIndex[widthAxis] = this->m_SliceSizeX;
  if( this->m_ClickIndex[heightAxis] > this->m_SliceSizeY ) this->m_ClickIndex[heightAxis] = this->m_SliceSizeY;

  for( unsigned int i=0; i<3; i++)
    {
    if(this->m_ClickIndex[i] < 0 )
      {
      this->m_ClickIndex[i] = 0;
      }
    }

  this->m_SliceNumber[widthAxis] = (unsigned int)(this->m_ClickIndex[widthAxis]);
  this->m_SliceNumber[heightAxis] = (unsigned int)(this->m_ClickIndex[heightAxis]);
  this->m_SliceNumber[depthAxis] = (unsigned int)(this->m_ClickIndex[depthAxis]);

  this->m_ClickPosition[depthAxis] = this->m_ClickIndex[depthAxis]*m_Spacing[depthAxis];
  this->m_ClickPosition[widthAxis] = this->m_ClickIndex[widthAxis]*m_Spacing[widthAxis];
  this->m_ClickPosition[heightAxis] = this->m_ClickIndex[heightAxis]*m_Spacing[heightAxis];

  //updating the slice number along each axis for each slicer method

  MethodListType::iterator it,end;
 
  it = m_MethodList.begin();
  end = m_MethodList.end();

  for(; it != end; it++ )
    {
    MethodType * method = dynamic_cast<MethodType*>((*it).GetPointer());
    
    if( method )
      {
      method->SetSliceNumber(this->m_SliceNumber[0],0);
      method->SetSliceNumber(this->m_SliceNumber[1],1);
      method->SetSliceNumber(this->m_SliceNumber[2],2);
      }
    }    
}

itk::Vector<double,3> GlRendererSlicer
::GetClickIndex()
{
  return this->m_ClickIndex;
}

itk::Vector<double,3> GlRendererSlicer
::GetClickPosition()
{
  return this->m_ClickPosition;
}

void 
GlRendererSlicer
::SetAxisOpacity( float opacity )
  {
  this->m_AxisOpacity = opacity;
  this->draw();
  }
  
float 
GlRendererSlicer
::GetAxisOpacity() const
{
  return this->m_AxisOpacity;
}

void 
GlRendererSlicer
::SetAxisColor( unsigned int i, float r, float g, float b )
{
  if( i<3 )
    {
    this->m_AxisColor[i][0] = r;
    this->m_AxisColor[i][0] = g;
    this->m_AxisColor[i][0] = b;
    this->draw();
    }
}

void 
GlRendererSlicer
::FlipX( bool value )
  {
  this->m_FlipX = value;
  }

bool
GlRendererSlicer
::FlipX() const
  {
  return this->m_FlipX;
  }

void 
GlRendererSlicer
::FlipY( bool value )
  {
  this->m_FlipY = value;
  }

bool
GlRendererSlicer
::FlipY() const
  {
  return this->m_FlipY;
  }

} //end of namespace sov
