/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderMethod3DFactory.h,v $
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
#ifndef __GlRenderMethod3DFactory_h
#define __GlRenderMethod3DFactory_h

#include <sovViewerFactoryBase.h>
#include <itkSpatialObject.h>

namespace sov
{

using namespace itk;

class GlRenderMethod3DFactory
: public ViewerFactoryBase
{

public:

  typedef GlRenderMethod3DFactory     Self;
  typedef ViewerFactoryBase           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  
  typedef SpatialObject<3,AffineTransform<double,3>,3> ObjectType;

  itkFactorylessNewMacro( Self );
  
  itkTypeMacro( GlRenderMethod3DFactory, ViewerFactoryBase );
  
  virtual const char *  GetITKSourceVersion( void ) const;

  virtual const char *  GetDescription( void ) const;

  static void RegisterOneFactory(void)
  {
    Self::Pointer renderMethodFactory = Self::New();
    ViewerFactoryBase::RegisterFactory(renderMethodFactory);
  }

protected:

  virtual LightObject::Pointer CreateObject( const char * );

    /** Constructor */
  GlRenderMethod3DFactory( void );

  /** Destructor */
  ~GlRenderMethod3DFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  GlRenderMethod3DFactory( const Self & );    //purposely not implemented 
  void operator=( const Self & );             //purposely not implemented

};

} //end of namespace sov


#endif //__GlRenderMethod3DFactory_h
