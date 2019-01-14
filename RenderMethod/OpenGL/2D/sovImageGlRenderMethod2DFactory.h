/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageGlRenderMethod2DFactory.h,v $
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
#ifndef _sovImageGlRenderMethod2DFactory_h
#define _sovImageGlRenderMethod2DFactory_h

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
class ImageGlRenderMethod2DFactory
: public ViewerFactoryBase
{

public:

  /* Typedefs */
  typedef ImageGlRenderMethod2DFactory          Self;
  typedef ViewerFactoryBase                     Superclass;
  typedef itk::SmartPointer< Self >             Pointer;
  typedef itk::SmartPointer< const Self >       ConstPointer;
  typedef itk::Image<ImagePixelType,2>          ImageType;
  
  itkFactorylessNewMacro( Self );
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( ImageGlRenderMethod2DFactory, ViewerFactoryBase );
  
  /** */
  virtual const char *  GetITKSourceVersion( void ) const; 

  /** */
  virtual const char *  GetDescription( void ) const;

  static void RegisterOneFactory(void)
  {
    itk::SmartPointer<ImageGlRenderMethod2DFactory> renderMethodFactory;
    renderMethodFactory = Self::New();
    ViewerFactoryBase::RegisterFactory(renderMethodFactory);
  }

protected:

  //virtual LightObject::Pointer CreateObject( const char * );

  /** Constructor */
  ImageGlRenderMethod2DFactory( void );

  /** Destructor */
  ~ImageGlRenderMethod2DFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  ImageGlRenderMethod2DFactory( const Self & );  //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

} // end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageGlRenderMethod2DFactory.txx"
#endif


#endif //_CenterLineTubeOpenVTKRenderMethodFactory_h
