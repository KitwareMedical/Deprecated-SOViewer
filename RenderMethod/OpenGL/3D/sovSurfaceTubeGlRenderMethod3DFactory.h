/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceTubeGlRenderMethod3DFactory.h,v $
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
#ifndef _SurfaceTubeOpenGLRenderMethodFactory_h
#define _SurfaceTubeOpenGLRenderMethodFactory_h

#include "sovSurfaceTubeGlRenderMethod3D.h"

#include <sovViewerFactoryBase.h>
#include <itkIndent.h>

namespace sov
{

/** \class SORenderMethodFactory
 * \brief Implementation of an object factory to create SORenderMethod instances.
 *
 * This factory create a render method for each type of spatial object. If no method
 * has been found for a type of spatial object, then no instance is created.
 *
 * \also ViewerFactoryBase SORenderMethod */

class SurfaceTubeGlRenderMethod3DFactory
: public ViewerFactoryBase
{

public:

  /* Typedefs */
  typedef SurfaceTubeGlRenderMethod3DFactory        Self;
  typedef ViewerFactoryBase                         Superclass;
  typedef SmartPointer< Self >                      Pointer;
  typedef SmartPointer< const Self >                ConstPointer;
  
  itkFactorylessNewMacro( Self );
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( SurfaceTubeGlRenderMethod3DFactory, ViewerFactoryBase );
  
  /** */
  virtual const char *  GetITKSourceVersion( void ) const; 

  /** */
  virtual const char *  GetDescription( void ) const;

  static void RegisterOneFactory(void)
  {
    Self::Pointer renderMethodFactory = Self::New();
    ViewerFactoryBase::RegisterFactory(renderMethodFactory);
  }

protected:

  virtual LightObject::Pointer CreateObject( const char * );

  /** Constructor */
  SurfaceTubeGlRenderMethod3DFactory( void );

  /** Destructor */
  ~SurfaceTubeGlRenderMethod3DFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  SurfaceTubeGlRenderMethod3DFactory( const Self & );  //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

} // end of namespace sov

#endif //_CenterLineTubeOpenGLRenderMethodFactory_h
