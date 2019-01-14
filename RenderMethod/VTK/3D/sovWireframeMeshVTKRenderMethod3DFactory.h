/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovWireframeMeshVTKRenderMethod3DFactory.h,v $
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
#ifndef _WireframeMeshOpenVTKRenderMethodFactory_h
#define _WireframeMeshOpenVTKRenderMethodFactory_h

#include "sovWireframeMeshVTKRenderMethod3D.h"

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
* \also ViewerFactoryBase SORenderMethod
*/

template <class MeshType>
class WireframeMeshVTKRenderMethod3DFactory
: public ViewerFactoryBase
{

public:

  /* Typedefs */
  typedef WireframeMeshVTKRenderMethod3DFactory     Self;
  typedef ViewerFactoryBase                         Superclass;
  typedef itk::SmartPointer< Self >                 Pointer;
  typedef itk::SmartPointer< const Self >           ConstPointer;
  
  itkFactorylessNewMacro( Self );
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( WireframeMeshVTKRenderMethod3DFactory, ViewerFactoryBase );
  
  /** */
  virtual const char *  GetITKSourceVersion( void ) const; 

  /** */
  virtual const char *  GetDescription( void ) const;

  static void RegisterOneFactory(void)
    {
    itk::SmartPointer<WireframeMeshVTKRenderMethod3DFactory> renderMethodFactory;
    renderMethodFactory = WireframeMeshVTKRenderMethod3DFactory<MeshType>::New();
    ViewerFactoryBase::RegisterFactory(renderMethodFactory);
    }

protected:

  /** Constructor */
  WireframeMeshVTKRenderMethod3DFactory( void );

  /** Destructor */
  ~WireframeMeshVTKRenderMethod3DFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  WireframeMeshVTKRenderMethod3DFactory( const Self & );  //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

} // end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovWireframeMeshVTKRenderMethod3DFactory.txx"
#endif

#endif //_sovWireframeMeshVTKRenderMethod3DFactory_h
