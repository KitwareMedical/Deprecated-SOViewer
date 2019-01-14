/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovFlGlDisplay.h,v $
  Language:  C++
  Date:      $Date: 2005-05-23 15:28:11 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __FlGlDisplay_h
#define __FlGlDisplay_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <FL/gl.h>

#include "FL/Fl_Gl_Window.H"
#include <sovDisplay.h>
#include "Fl_Gl_TrackBall_Window.h"
#include "sovMouseInteractor.h"
#include "sovKeyboardInteractor.h"
#include "sovNDimensionalRenderer.h"
#include <list>
#include "sovObjectViewerWin32Header.h"

#define NB_LIGHTS 3

namespace sov
{
  
class SOViewer_EXPORT FlGlDisplay
: public Fl_Gl_Window, Display
{

public:

  typedef FlGlDisplay                   Self;
  typedef Fl_Gl_Window                  Superclass;
  typedef NDimensionalRenderer          RendererType;
  typedef std::list< RendererType::Pointer >   RendererListType;
  typedef GLfloat                       ScalarType;

  FlGlDisplay( int x, int y, int w, int h, const char *label = 0 );

  ~FlGlDisplay();

  virtual void draw( void );

  void SetRendererList( RendererListType);

  const RendererListType & GetRendererList() const;

  void AddRenderer( RendererType * renderer );

  void SetMouseInteractor(MouseInteractor* interactor);
  void SetKeyboardInteractor(KeyboardInteractor* interactor);


  virtual void Update();
  virtual void Redraw() {this->redraw();}

  virtual int handle(int event);
  
  virtual void resize(int x,int y,int w,int h);

protected:

  void BuildAxisDisplayList();
  
  GLint m_AxisDisplayList;
  RendererListType    m_RendererList;
  ScalarType          m_BackgroundColor[3];
  ScalarType          m_LightModelAmbient[4];
  ScalarType          m_LightPosition[NB_LIGHTS][4];
  ScalarType          m_LightAmbient[NB_LIGHTS][4];
  ScalarType          m_LightDiffuse[NB_LIGHTS][4];
  ScalarType          m_LightSpecular[NB_LIGHTS][4];
  ScalarType          m_LightDirection[NB_LIGHTS][3];
  ScalarType          m_MatShininess;
  ScalarType          m_MatSpecular[4];
  bool                m_ShowAxis;
  MouseInteractor*    m_MouseInteractor;
  KeyboardInteractor* m_KeyboardInteractor;

};

} //end of namespace sov

#endif //__FlGlDisplay_h
