/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPhantomRenderMethodFactory.h,v $
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
#ifndef _PhantomRenderMethodFactory_h
#define _PhantomRenderMethodFactory_h

#include "sovNDimensionalRenderMethod.h"

#include <sovViewerFactoryBase.h>

using namespace itk;

namespace sov
{

/** \class SOPhantomRenderMethodFactory
* \brief Implementation of an object factory to create SORenderMethod instances.
*
* This factory create a render method for each type of spatial object. If no method
* has been found for a type of spatial object, then no instance is created.
*
* \also ViewerFactoryBase SORenderMethod
*/
template < unsigned int TDimension >
class ITK_EXPORT PhantomRenderMethodFactory : public Object
{

public:

  typedef PhantomRenderMethodFactory          Self;
  typedef Object                              Superclass;
  typedef SmartPointer< Self >                Pointer;
  typedef SmartPointer< const Self >          ConstPointer;
  typedef ::sov::RenderMethod<TDimension>     RenderMethodType;
  typedef typename RenderMethodType::Pointer  RenderMethodPointer;
  itkNewMacro( PhantomRenderMethodFactory );

  itkTypeMacro( PhantomRenderMethodFactory, Object );
  
  static RenderMethodPointer CreateRenderMethod( const char *, 
                                                 unsigned int dimension, 
                                                 const char* type,
                                                 const char* methodName,
                                                 const char* pixelType=NULL);
 
protected:

  /** Constructor */
  PhantomRenderMethodFactory( void );

  /** Destructor */
  ~PhantomRenderMethodFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  PhantomRenderMethodFactory( const Self & );    //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

} // end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovPhantomRenderMethodFactory.txx"
#endif


#endif //_PhantomRenderMethodFactory_h
