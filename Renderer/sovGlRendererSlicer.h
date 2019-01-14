/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRendererSlicer.h,v $
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
#ifndef __GlRendererSlicer_h
#define __GlRendererSlicer_h

#include "sovRenderer.h"
#include "sovGlRenderMethodSlicer.h"
#include "sovGlRenderMethodFactory.h"
#include "sovObjectViewerWin32Header.h"

namespace sov
{

/**
 *
 */

class SOViewer_EXPORT GlRendererSlicer
: public Renderer<3>
{

public:

  typedef GlRendererSlicer               Self;
  typedef SmartPointer< Self >           Pointer;
  typedef Renderer<3>                    Superclass;
  typedef GlRenderMethodSlicer           MethodType;
  //typedef std::list<MethodType*>         MethodListType;
  typedef Superclass::MethodType         SuperclassMethodType;
  typedef Superclass::SceneType          SceneType;
  typedef Superclass::ObjectType         ObjectType;
  typedef Superclass::ObjectListType     ObjectListType;
  typedef Superclass::VisualObjectListType VisualObjectListType;

  itkNewMacro( Self );
  itkTypeMacro( GlRendererSlicer, Renderer );

  void AddRenderMethod( SuperclassMethodType * );

  void draw();

  void SetImageSize(unsigned int x, unsigned int y);

  void SetSliceNumber(unsigned int slice);
  unsigned int GetSliceNumber(void){return m_SliceNumber[this->m_Orientation];}
  unsigned int GetSliceNumber(int orientation){return m_SliceNumber[orientation];}

  void SetOrientation(unsigned int orientation);
  unsigned int GetOrientation(void){return m_Orientation;}

  void SetOrientation(ObjectType * object, unsigned int orientation);

  void AddObject( ObjectType * , bool addChildren = true );
  void AddObject( ObjectType *, SuperclassMethodType * );

  void SetScene( SceneType * );

  MethodPointer GetRenderMethodViaFactory(const char * nametype, 
                                    unsigned int dimension, 
                                    const char* type,
                                    const char* methodName,
                                    const char* pixelType=NULL)
                                    {return GlRenderMethodFactory<3>::CreateRenderMethod( nametype, 
                                            3, type, methodName, pixelType );}

  void SetAxisOpacity( float opacity );
  
  float GetAxisOpacity() const;

  void SetAxisColor( unsigned int i, float r, float g, float b );

  void FlipX( bool value );
  
  void FlipY( bool value );
  
  bool FlipX() const;
  
  bool FlipY() const;

  void Click( int x, int y);
  
  itk::Vector<double,3> GetClickIndex();

  itk::Vector<double,3> GetClickPosition();

  void SetViewCrosshair(bool val) {m_ViewCrosshair = val;}
  bool GetViewCrosshair() {return m_ViewCrosshair;}

protected:

  bool m_FlipX;
  bool m_FlipY;

  GlRendererSlicer();
  ~GlRendererSlicer();

  void PrintSelf( std::ostream& , itk::Indent indent ) const;
  void AutoSetImageSize(unsigned int orientation);

  unsigned char * m_DisplayVolume;/** This is a data volume in wich every objects displays itself */
  unsigned int m_SliceSizeX;
  unsigned int m_SliceSizeY;
  unsigned int m_SliceNumber[3];
  unsigned int m_Orientation;

  double m_ViewZoom;
  float m_AxisOpacity;
  float m_AxisColor[3][3];

  //itk::Vector<double,2>  m_SliceSpacing;
  itk::Vector<double,3>  m_Spacing;
  itk::Vector<double,3>  m_ClickIndex;
  itk::Vector<double,3>  m_ClickPosition;
  bool m_ViewCrosshair;
};

} //end of namespace sov

#endif //__GlRendererSlicer_h
