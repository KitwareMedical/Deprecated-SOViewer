/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageVTKRenderMethodSlicerFactory.h,v $
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
#ifndef _sovImageVTKRenderMethodSlicerFactory_h
#define _sovImageVTKRenderMethodSlicerFactory_h

#include "sovImageVTKRenderMethodSlicerFactory.h"

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

template <class ImagePixelType>
class ImageVTKRenderMethodSlicerFactory
: public ViewerFactoryBase
{

public:

  /* Typedefs */
  typedef ImageVTKRenderMethodSlicerFactory     Self;
  typedef ViewerFactoryBase                     Superclass;
  typedef itk::SmartPointer< Self >             Pointer;
  typedef itk::SmartPointer< const Self >       ConstPointer;
  typedef itk::Image<ImagePixelType,3>          ImageType;
  
  itkFactorylessNewMacro( Self );
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( ImageVTKRenderMethodSlicerFactoryFactory, ViewerFactoryBase );
  
  /** */
  virtual const char *  GetITKSourceVersion( void ) const; 

  /** */
  virtual const char *  GetDescription( void ) const;

  static void RegisterOneFactory(void)
  {
    itk::SmartPointer<ImageVTKRenderMethodSlicerFactory> renderMethodFactory;
    renderMethodFactory = ImageVTKRenderMethodSlicerFactory::New();
    ViewerFactoryBase::RegisterFactory(renderMethodFactory);
  }

protected:

  //virtual LightObject::Pointer CreateObject( const char * );

  /** Constructor */
  ImageVTKRenderMethodSlicerFactory( void );

  /** Destructor */
  ~ImageVTKRenderMethodSlicerFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  ImageVTKRenderMethodSlicerFactory( const Self & );  //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

} // end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageVTKRenderMethodSlicerFactory.txx"
#endif


#endif //_CenterLineTubeOpenVTKRenderMethodFactory_h
