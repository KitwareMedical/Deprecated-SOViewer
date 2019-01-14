/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovTubeGlRenderMethod3D.h,v $
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
#ifndef _TubeGlRenderMethod3D_h
#define _TubeGlRenderMethod3D_h

#include "sovGlRenderMethod3D.h"

#include <itkTubeSpatialObject.h>

namespace sov
{

  class TubeGlRenderMethod3D
  : public GlRenderMethod3D
  {

  public:

    typedef TubeGlRenderMethod3D        Self;
    typedef GlRenderMethod3D            Superclass;
    typedef SmartPointer< Self >        Pointer;
    typedef SmartPointer< const Self >  ConstPointer;

    typedef itk::TubeSpatialObject      TubeType;
    typedef Superclass::ObjectType      ObjectType;
    typedef Superclass::ObjectListType  ObjectListType;

    itkNewMacro( TubeGlRenderMethod3D );

    itkTypeMacro( TubeGlRenderMethod3D, GlRenderMethod3D );

    virtual void draw( void );

    bool IsSupported( Pointer object ) const {};

  protected:

    TubeGlRenderMethod3D( void );

    ~TubeGlRenderMethod3D( void );

    void PrintSelf (std::ostream &os, Indent indent) const;

    virtual void Update();
  };

}

#endif //_TubeGlRenderMethod3D_h
