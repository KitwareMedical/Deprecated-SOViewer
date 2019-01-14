/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: Fl_Gl_TrackBall_Window.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef Fl_Gl_TrackBall_Window_h
#define Fl_Gl_TrackBall_Window_h

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include "Trackball.h"

class Fl_Gl_TrackBall_Window : public Fl_Gl_Window 
{

public:


  Fl_Gl_TrackBall_Window(int x, int y, int w, int h, const char *l = 0)
  : Fl_Gl_Window(x, y, w, h, l), m_Trackball(m_Mouse) { init(); }
  Fl_Gl_TrackBall_Window(int w, int h, const char *l = 0)
  : Fl_Gl_Window(w, h, l), m_Trackball(m_Mouse) { init(); }
  ~Fl_Gl_TrackBall_Window();

  void DoTransform(); // Call this from draw() to transform the view

  // Call idle_redraw(TRUE) to force a redraw() every time idle() is
  // called.  It is initially FALSE.
  void DoIdleRedraw(bool b) { m_IsIdleRedrawing = b; }
  Mouse     m_Mouse;
  Trackball m_Trackball;
  
protected:

  int handle(int);
  void init(void);
  static void idle_cb(Fl_Gl_TrackBall_Window *);
  void idle(void);
  bool m_IsIdleRedrawing;    // call redraw() in any case
};

#endif
