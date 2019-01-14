/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobGlRenderMethodSlicer.h,v $
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
#ifndef sov_BlobGlRenderMethodSlicer_h
#define sov_BlobGlRenderMethodSlicer_h

#include "sovGlRenderMethodSlicer.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

class SOVOpenGLRenderMethod_EXPORT BlobGlRenderMethodSlicer
: public GlRenderMethodSlicer
{

public:


  typedef BlobGlRenderMethodSlicer        Self;
  typedef GlRenderMethodSlicer            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  itkNewMacro( BlobGlRenderMethodSlicer );

  itkTypeMacro( BlobGlRenderMethodSlicer, GlRenderMethod3D );

  virtual void draw( void );

protected:

  BlobGlRenderMethodSlicer( void );
  ~BlobGlRenderMethodSlicer( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update(); 
  
};

}

#endif //_BlobGlRenderMethodSlicer_h
