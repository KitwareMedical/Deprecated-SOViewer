/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: Fl_Gl_TrackBall_Window.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "Fl_Gl_TrackBall_Window.h"

/**  Initialize the trackball window  */
void Fl_Gl_TrackBall_Window::init()
{
	// Add an idle callback that calls this object's idle()
	Fl::add_idle((void (*)(void *))idle_cb, this);

	m_IsIdleRedrawing = false;
	m_Trackball.Reset();
  m_Mouse.SetWindowSize(w(), h());
}

/** Destructor */
Fl_Gl_TrackBall_Window::~Fl_Gl_TrackBall_Window() 
{
	Fl::remove_idle((void (*)(void *))idle_cb);
}


/** 
 * Handle Method
 * The handle() method just records the events, and the idle callback
 * does the actual work.
 */
int Fl_Gl_TrackBall_Window::handle(int e) 
{
	switch (e) 
  {
	  case FL_PUSH:
      m_Mouse.UpdatePos(Fl::event_x(),Fl::event_y());
      m_Mouse.UpdatePos(Fl::event_x(),Fl::event_y());
      m_Mouse.UpdateButtons(Fl::event_state() & BUTTON_MASK);
		  return 1;
	  case FL_RELEASE:
      m_Mouse.UpdateButtons(Fl::event_state() & BUTTON_MASK);
		  return 1;
	  case FL_DRAG:
      m_Mouse.UpdatePos(Fl::event_x(),Fl::event_y());
      m_Mouse.UpdateButtons(Fl::event_state() & BUTTON_MASK);
		  return 1;
    default:
      // some other event, which might be resize
      m_Mouse.SetWindowSize(w(), h());
	}
	return 0;
}

/** Idle Callback */
void Fl_Gl_TrackBall_Window::idle_cb(Fl_Gl_TrackBall_Window *t) 
{
	t->idle();
}

/** Idle function */
void Fl_Gl_TrackBall_Window::idle() 
{
  m_Trackball.Update();
	if (m_IsIdleRedrawing || m_Trackball.m_IsSpinning || m_Trackball.m_IsChanged) 
  {
		redraw();
    m_Trackball.m_IsChanged = false;
  }
}
