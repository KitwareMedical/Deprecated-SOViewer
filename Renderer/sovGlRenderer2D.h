/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderer2D.h,v $
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
#ifndef __GlRenderer2D_h
#define __GlRenderer2D_h

#include "sovRenderer.h"
#include "sovGlRenderMethod2D.h"
#include "sovGlRenderMethodFactory.h"
#include "sovObjectViewerWin32Header.h"

namespace sov
{

/**
 *
 */

class SOViewer_EXPORT GlRenderer2D
: public Renderer<2>
{

public:

  typedef GlRenderer2D                  Self;
  typedef SmartPointer< Self >          Pointer;
  typedef Renderer<2>                   Superclass;
  typedef GlRenderMethod2D              MethodType;
  typedef Superclass::MethodType        SuperclassMethodType;
  typedef Superclass::SceneType         SceneType;
  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;

  itkNewMacro( Self );
  itkTypeMacro( GlRenderer2D, Renderer );

  void AddRenderMethod( SuperclassMethodType * );

  void draw();

  void SetImageSize(unsigned int x, unsigned int y);

  void AddObject( ObjectType * , bool addChildren = true );
  void AddObject( ObjectType *, SuperclassMethodType * );

  void SetScene( SceneType * );

  //void Update();
  
  MethodPointer GetRenderMethodViaFactory(const char * nametype, 
                                    unsigned int dimension, 
                                    const char* type,
                                    const char* methodName,
                                    const char* pixelType=NULL)
                                    {return GlRenderMethodFactory<2>::CreateRenderMethod( nametype, 
                                            2, type, methodName, pixelType );}

protected:

  GlRenderer2D();
  ~GlRenderer2D();

  void PrintSelf( std::ostream& , itk::Indent indent ) const;

  unsigned char * m_DisplayVolume;/** This is a data volume in wich every objects displays itself */
  unsigned int m_SliceSizeX;
  unsigned int m_SliceSizeY;
  itk::Vector<double,2>  m_SliceSpacing;

};

} //end of namespace sov

#endif //__GlRenderer2D_h
