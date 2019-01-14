/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovQtGlDisplay.cxx,v $
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
#include "sovQtGlDisplay.h"
 

QtGlDisplay::QtGlDisplay( QWidget *parent, const char *name) : QGLWidget(parent, name)
{
  m_MouseInteractor = new sov::MouseInteractor;
  m_KeyboardInteractor = new sov::KeyboardInteractor;
}
  
QtGlDisplay::
QtGlDisplay( QGLFormat glf, QWidget *parent, const char *name)
: QGLWidget(glf,parent, name)
{
  m_MouseInteractor = new sov::MouseInteractor;
  m_KeyboardInteractor = new sov::KeyboardInteractor;
}
  
/** Set the list of renderers*/
void 
QtGlDisplay
::SetRendererList( RendererListType list )
{
  m_RendererList = list;
}

/** Destructor */
QtGlDisplay
::~QtGlDisplay()
{
  delete m_MouseInteractor;
  delete m_KeyboardInteractor;
}

/** Add a renderer to the scene */
void
QtGlDisplay
::AddRenderer( RendererType * renderer )
{
  renderer->SetWidth(this->width());
  renderer->SetHeight(this->height());
  renderer->UpdateDisplaySize();

  m_RendererList.push_back(renderer);
  m_RendererList.unique();
}


/** Get the list of renderers */
QtGlDisplay::RendererListType
QtGlDisplay
::GetRendererList()
{
  return m_RendererList;
}

/** Set the mouse interactor */
void 
QtGlDisplay
::SetMouseInteractor(sov::MouseInteractor* interactor)
{
  m_MouseInteractor = interactor;
}

/** Set the keyboard interactor */
void 
QtGlDisplay
::SetKeyboardInteractor(sov::KeyboardInteractor* interactor)
{
  m_KeyboardInteractor = interactor;
}

void 
QtGlDisplay::Update()
{
  RendererListType::const_iterator it = m_RendererList.begin();
  while(it != m_RendererList.end())
  {
    (*it)->Update();
    it++;
  }
  this->updateGL();
}


/** Set up the OpenGL view port, matrix mode, etc. */
void QtGlDisplay::resizeGL( int w, int h )
{
  RendererListType::const_iterator it = m_RendererList.begin();
  while(it != m_RendererList.end())
  {
    (*it)->SetWidth(this->width());
    (*it)->SetHeight(this->height());
    (*it)->UpdateDisplaySize();
    it++;
  }
}

/** Initialize the OpenGL Window */
void QtGlDisplay::initializeGL() 
{
  glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0,this->width(),0.0,this->height());
  glViewport(0,0,this->width(),this->height());

  // Establish the model view matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
}

/** Draw the scene */
void QtGlDisplay::paintGL(void)
{
  glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0,this->width(),0.0,this->height());
  glViewport(0,0,this->width(),this->height());

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearColor((float)1.0, (float)0.0, (float)0.0, (float)0.0);

  glPushMatrix();
  glTranslated(0.5 * this->width(),0.5 * this->height(),0);

  RendererListType::const_iterator it,end;
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


/**  Catches the mouse move to react appropriate */
void QtGlDisplay::mouseMoveEvent( QMouseEvent *event ) 
{
  m_MouseInteractor->Drag(event->x(),event->y(),event->button());
  this->updateGL();
}

/**  Catches the mouse press to react appropriate */
void QtGlDisplay::mousePressEvent( QMouseEvent *event ) 
{
  if( event->button() & LeftButton ) 
  {
    m_MouseInteractor->Push(event->x(),event->y(),0);
    if( event->state() & ShiftButton ) 
    {
      
    }
  }
  else if( event->button() & MidButton ) 
  {
    m_MouseInteractor->Push(event->x(),event->y(),2); 
  }
  else if( event->button() & RightButton )
  {
    m_MouseInteractor->Push(event->x(),event->y(),1);
  }

  this->updateGL();
}
