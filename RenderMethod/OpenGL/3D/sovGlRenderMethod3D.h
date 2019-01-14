/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderMethod3D.h,v $
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
#ifndef __GlRenderMethod3D_h
#define __GlRenderMethod3D_h

#include "sovRenderMethod3D.h"

//#include <FL/gl.h>

namespace sov
{

class GlRenderMethod3D
: public RenderMethod3D
{

public:

  typedef GlRenderMethod3D              Self;
  typedef RenderMethod3D                Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;
  
  itkNewMacro( GlRenderMethod3D );

  itkTypeMacro( GlRenderMethod3D, RenderMethod3D );

  virtual void draw();
  
  bool IsSupported( const char* object ) const;

protected:

  GlRenderMethod3D();
  
  ~GlRenderMethod3D();
 
  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();
};

}
#endif //__GlRenderMethod3D_h
