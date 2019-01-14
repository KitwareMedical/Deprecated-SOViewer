/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSpherePhantomRenderMethod.h,v $
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
#ifndef _SpherePhantomRenderMethod_h
#define _SpherePhantomRenderMethod_h

#include "sovPhantomRenderMethod.h"
#include <itkEllipseSpatialObject.h>

namespace sov
{

class SpherePhantomRenderMethod
: public PhantomRenderMethod
{

public:

  typedef SpherePhantomRenderMethod     Self;
  typedef PhantomRenderMethod           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef itk::EllipseSpatialObject<3>  SphereType;
  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;

  itkNewMacro( SpherePhantomRenderMethod );
  itkTypeMacro( SpherePhantomRenderMethod, PhantomRenderMethod );

  virtual void Update();
  
protected:

  SpherePhantomRenderMethod( void );
  ~SpherePhantomRenderMethod( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  
};

}

#endif //_SpherePhantomRenderMethod_h
