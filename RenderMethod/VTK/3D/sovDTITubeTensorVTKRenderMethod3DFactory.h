/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovDTITubeTensorVTKRenderMethod3DFactory.h,v $
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
#ifndef _sovDTITubeTensorVTKRenderMethod3DFactory_h
#define _sovDTITubeTensorVTKRenderMethod3DFactory_h

#include "sovDTITubeTensorVTKRenderMethod3D.h"

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

class SOVVTKRenderMethod_EXPORT DTITubeTensorVTKRenderMethod3DFactory
: public ViewerFactoryBase
{

public:

  /* Typedefs */
  typedef DTITubeTensorVTKRenderMethod3DFactory    Self;
  typedef ViewerFactoryBase                           Superclass;
  typedef SmartPointer< Self >                        Pointer;
  typedef SmartPointer< const Self >                  ConstPointer;
  
  itkFactorylessNewMacro( Self );
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( DTITubeTensorVTKRenderMethod3DFactory, ViewerFactoryBase );
  
  /** */
  virtual const char *  GetITKSourceVersion( void ) const; 

  /** */
  virtual const char *  GetDescription( void ) const;

  static void RegisterOneFactory(void)
  {
    DTITubeTensorVTKRenderMethod3DFactory::Pointer renderMethodFactory = DTITubeTensorVTKRenderMethod3DFactory::New();
    ViewerFactoryBase::RegisterFactory(renderMethodFactory);
  }

protected:

  //virtual LightObject::Pointer CreateObject( const char * );

  /** Constructor */
  DTITubeTensorVTKRenderMethod3DFactory( void );

  /** Destructor */
  ~DTITubeTensorVTKRenderMethod3DFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  DTITubeTensorVTKRenderMethod3DFactory( const Self & );  //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

} // end of namespace sov

#endif //_CenterLineTubeOpenVTKRenderMethodFactory_h
