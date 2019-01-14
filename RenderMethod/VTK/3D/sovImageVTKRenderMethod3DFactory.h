/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageVTKRenderMethod3DFactory.h,v $
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
#ifndef _sovImageVTKRenderMethod3DFactory_h
#define _sovImageVTKRenderMethod3DFactory_h

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
class ImageVTKRenderMethod3DFactory
: public ViewerFactoryBase
{

public:

  /* Typedefs */
  typedef ImageVTKRenderMethod3DFactory         Self;
  typedef ViewerFactoryBase                     Superclass;
  typedef itk::SmartPointer< Self >             Pointer;
  typedef itk::SmartPointer< const Self >       ConstPointer;
  typedef itk::Image<ImagePixelType,3>          ImageType;
  
  itkFactorylessNewMacro( Self );
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( ImageVTKRenderMethod3DFactory, ViewerFactoryBase );
  
  /** */
  virtual const char *  GetITKSourceVersion( void ) const; 

  /** */
  virtual const char *  GetDescription( void ) const;

  static void RegisterOneFactory(void)
    {
    itk::SmartPointer<ImageVTKRenderMethod3DFactory> metaFactory;
    metaFactory= ImageVTKRenderMethod3DFactory<ImagePixelType>::New();
    ViewerFactoryBase::RegisterFactory(metaFactory);
    }

protected:

  //virtual LightObject::Pointer CreateObject( const char * );

  /** Constructor */
  ImageVTKRenderMethod3DFactory( void );

  /** Destructor */
  ~ImageVTKRenderMethod3DFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  ImageVTKRenderMethod3DFactory( const Self & );  //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

} // end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageVTKRenderMethod3DFactory.txx"
#endif


#endif //_CenterLineTubeOpenVTKRenderMethodFactory_h
