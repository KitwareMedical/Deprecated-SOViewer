/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovFlGlDisplay.cxx,v $
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
#include "sovFlGlDisplay.h"

#include <iostream>
#include <GL/glu.h>
#include <itkVector.h>
#include <itkImageRegionIteratorWithIndex.h>

namespace sov
{

FlGlDisplay
::FlGlDisplay( int x, int y, int w, int h, const char * label )
: Fl_Gl_Window( x, y, w, h, label)
{
  m_ShowAxis = false;
  m_MouseInteractor = NULL;
  m_KeyboardInteractor = NULL;
}


FlGlDisplay
::~FlGlDisplay()
{
}

/** Set the list of renderers*/
void 
FlGlDisplay
::SetRendererList( RendererListType list )
{
  m_RendererList = list;
}

/** Get the list of renderers */
const FlGlDisplay::RendererListType &
FlGlDisplay
::GetRendererList() const
{
  return m_RendererList;
}

void 
FlGlDisplay
::SetMouseInteractor(MouseInteractor* interactor)
{
  m_MouseInteractor = interactor;
  //m_MouseInteractor->SetRendererList(&m_RendererList);
  m_MouseInteractor->SetDisplay(this);
}

void 
FlGlDisplay
::SetKeyboardInteractor(KeyboardInteractor* interactor)
{
  m_KeyboardInteractor = interactor;
  //m_Interactor->SetRendererList(&m_RendererList);
  m_KeyboardInteractor->SetDisplay(this);
}

void
FlGlDisplay
::draw()
{
  // Set up the projection if necessary
  if(!valid()) 
  {
    // The window will have coordinates (0,0) to (w,h), i.e. the same as the window
    // coordinates.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,w(),0.0,h());
    glViewport(0,0,w(),h());

    // Establish the model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  // Clear the display
  glClearColor(0.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    

  // Set up lighting attributes
  glPushAttrib(GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT | 
               GL_PIXEL_MODE_BIT | GL_TEXTURE_BIT );  
  
  
  glDisable(GL_LIGHTING);

  // glDisable(GL_DEPTH);

  // Prepare for overlay drawing.  The model view is set up to correspond
  // to pixel coordinates of the slice
  glPushMatrix();
  glTranslated(0.5 * w(),0.5 * h(),0.0);


  RendererListType::iterator it,end;
  it = m_RendererList.begin();
  end = m_RendererList.end();
  for(; it != end; it++ )
  {
    (*it)->draw();
  }

  // Clean up the GL state
  glPopMatrix();
  glPopAttrib();

  glFlush();
}

/** Add a renderer to the scene */
void
FlGlDisplay
::AddRenderer( RendererType * renderer )
{
  renderer->SetReferenceCount(2); // TRICKY !!!

  renderer->SetWidth(this->w());
  renderer->SetHeight(this->h());
  renderer->UpdateDisplaySize();

  m_RendererList.push_back(renderer);
  m_RendererList.unique();
}

/** Refresh the display */
void 
FlGlDisplay::Update()
{
  RendererListType::const_iterator it = m_RendererList.begin();
  while(it != m_RendererList.end())
  {
    (*it)->Update();
    it++;
  }
  this->redraw();
}

/** Resize the display */
void 
FlGlDisplay::resize(int x,int y,int w,int h)
{
  Superclass::resize(x,y,w,h);
  RendererListType::const_iterator it = m_RendererList.begin();
  while(it != m_RendererList.end())
  {
    (*it)->SetWidth(this->w());
    (*it)->SetHeight(this->h());
    (*it)->UpdateDisplaySize();
    it++;
  }
}


/** Refresh the display */
int
FlGlDisplay
::handle(int event)
{
  bool val = Superclass::handle(event);
  int x = Fl::event_x();
  int y = Fl::event_y();
  int key;
  
  switch(event)
  {
    case FL_PUSH:
      if(m_MouseInteractor)
        {
        m_MouseInteractor->Push(x,y,Fl::event_button());
        }
      break;
    case FL_DRAG:
      if(m_MouseInteractor)
        {
        m_MouseInteractor->Drag(x,y,Fl::event_button());
        }
      break;
    case FL_RELEASE: 
      if(m_MouseInteractor)
        {
        m_MouseInteractor->Release(x,y,Fl::event_button());
        }
      break;
    case FL_SHORTCUT:
      key = Fl::event_text()[0];
      if(m_KeyboardInteractor)
        {
        val = m_KeyboardInteractor->Shortcut(key);
        }
      break;
    default:
      if(m_MouseInteractor)
        {
        m_MouseInteractor->Handle(x,y);
        }
      break;
  }
  //this->draw();
  //this->make_current();
  return val;
}

} //end of namespace sov
