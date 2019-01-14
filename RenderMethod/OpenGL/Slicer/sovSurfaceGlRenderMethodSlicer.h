/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceGlRenderMethodSlicer.h,v $
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
#ifndef sov_SurfaceGlRenderMethodSlicer_h
#define sov_SurfaceGlRenderMethodSlicer_h

#include "sovGlRenderMethodSlicer.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

class SOVOpenGLRenderMethod_EXPORT SurfaceGlRenderMethodSlicer
: public GlRenderMethodSlicer
{

public:


  typedef SurfaceGlRenderMethodSlicer        Self;
  typedef GlRenderMethodSlicer            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  itkNewMacro( SurfaceGlRenderMethodSlicer );

  itkTypeMacro( SurfaceGlRenderMethodSlicer, GlRenderMethod3D );

  virtual void draw( void );

protected:

  SurfaceGlRenderMethodSlicer( void );
  ~SurfaceGlRenderMethodSlicer( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update(); 
  
};

}

#endif //_SurfaceGlRenderMethodSlicer_h
