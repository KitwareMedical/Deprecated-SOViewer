/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderMethod2D.h,v $
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
#ifndef __GlRenderMethod2D_h
#define __GlRenderMethod2D_h

#include "sovRenderMethod.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

class SOVOpenGLRenderMethod_EXPORT GlRenderMethod2D
: public RenderMethod<2>
{

public:

  typedef GlRenderMethod2D          Self;
  typedef RenderMethod<2>                Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;
  
  itkNewMacro( GlRenderMethod2D );

  itkTypeMacro( GlRenderMethod2D, RenderMethodSlicer );

  virtual void draw();
  
  bool IsSupported( const char* object ) const;

  void SetDisplayVolume(unsigned char * volume ){};

  void SetImageSize(unsigned int x, unsigned int y){m_SliceSizeX=x,m_SliceSizeY=y;};


protected:

  GlRenderMethod2D();
  
  ~GlRenderMethod2D();
 
  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

  unsigned char* m_DisplayVolume;
  unsigned char* m_DisplaySlice;
  unsigned int m_SliceSizeX;
  unsigned int m_SliceSizeY;
  unsigned int m_SliceNumber;
  unsigned int m_Orientation;
};

}
#endif //__GlRenderMethod2D_h
