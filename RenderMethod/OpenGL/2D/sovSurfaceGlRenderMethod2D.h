/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceGlRenderMethod2D.h,v $
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
#ifndef sov_SurfaceGlRenderMethod2D_h
#define sov_SurfaceGlRenderMethod2D_h

#include "sovGlRenderMethod2D.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

class SOVOpenGLRenderMethod_EXPORT SurfaceGlRenderMethod2D
: public GlRenderMethod2D
{

public:


  typedef SurfaceGlRenderMethod2D        Self;
  typedef GlRenderMethod2D            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  itkNewMacro( SurfaceGlRenderMethod2D );

  itkTypeMacro( SurfaceGlRenderMethod2D, GlRenderMethod3D );

  virtual void draw( void );

protected:

  SurfaceGlRenderMethod2D( void );
  ~SurfaceGlRenderMethod2D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update(); 
};

}

#endif //_SurfaceGlRenderMethod2D_h
