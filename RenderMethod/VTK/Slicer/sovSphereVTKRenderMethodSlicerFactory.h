/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSphereVTKRenderMethodSlicerFactory.h,v $
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
#ifndef _SphereVTKRenderMethodSlicerFactory_h
#define _SphereVTKRenderMethodSlicerFactory_h

#include "sovSphereVTKRenderMethodSlicer.h"
#include <sovViewerFactoryBase.h>
#include <itkIndent.h>
#include <sovVTKWin32Header.h>

namespace sov
{

/** \class SORenderMethodFactory
* \brief Implementation of an object factory to create SORenderMethod instances.
*
* This factory create a render method for each type of spatial object. If no method
* has been found for a type of spatial object, then no instance is created.
*
* \also ViewerFactoryBase SORenderMethod
*/

class SOVVTKRenderMethod_EXPORT SphereVTKRenderMethodSlicerFactory
: public ViewerFactoryBase
{

public:

  /* Typedefs */
  typedef SphereVTKRenderMethodSlicerFactory        Self;
  typedef ViewerFactoryBase                     Superclass;
  typedef SmartPointer< Self >                  Pointer;
  typedef SmartPointer< const Self >            ConstPointer;
  
  itkFactorylessNewMacro( Self );
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( SphereVTKRenderMethodSlicerFactory, ViewerFactoryBase );
  
  /** */
  virtual const char *  GetITKSourceVersion( void ) const; 

  /** */
  virtual const char *  GetDescription( void ) const;

  static void RegisterOneFactory(void)
  {
    SphereVTKRenderMethodSlicerFactory::Pointer renderMethodFactory = SphereVTKRenderMethodSlicerFactory::New();
    ViewerFactoryBase::RegisterFactory(renderMethodFactory);
  }

protected:

  //virtual LightObject::Pointer CreateObject( const char * );

  /** Constructor */
  SphereVTKRenderMethodSlicerFactory( void );

  /** Destructor */
  ~SphereVTKRenderMethodSlicerFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  SphereVTKRenderMethodSlicerFactory( const Self & );  //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

} // end of namespace sov

#endif //_CenterLineTubeOpenVTKRenderMethodFactory_h
