/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRenderMethodFactory.h,v $
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
#ifndef _VTKRenderMethodFactory_h
#define _VTKRenderMethodFactory_h

#include "sovNDimensionalRenderMethod.h"

#include <sovViewerFactoryBase.h>

using namespace itk;

namespace sov
{

/** \class SOVTKRenderMethodFactory
* \brief Implementation of an object factory to create SORenderMethod instances.
*
* This factory create a render method for each type of spatial object. If no method
* has been found for a type of spatial object, then no instance is created.
*
* \also ViewerFactoryBase SORenderMethod
*/
template < unsigned int TDimension >
class ITK_EXPORT VTKRenderMethodFactory : public Object
{

public:

  typedef VTKRenderMethodFactory              Self;
  typedef Object                              Superclass;
  typedef SmartPointer< Self >                Pointer;
  typedef SmartPointer< const Self >          ConstPointer;
  typedef ::sov::RenderMethod<TDimension>     RenderMethodType;
  typedef typename RenderMethodType::Pointer  RenderMethodPointer;
  itkNewMacro( VTKRenderMethodFactory );

  itkTypeMacro( VTKRenderMethodFactory, Object );
  
  static RenderMethodPointer CreateRenderMethod( const char *, 
                                                 unsigned int dimension, 
                                                 const char* type,
                                                 const char* methodName,
                                                 const char* pixelType=NULL);
 
protected:

  /** Constructor */
  VTKRenderMethodFactory( void );

  /** Destructor */
  ~VTKRenderMethodFactory( void );

  /** Print out the object properties. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:

  VTKRenderMethodFactory( const Self & );    //purposely not implemented 
  void operator=( const Self & );         //purposely not implemented

};

} // end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovVTKRenderMethodFactory.txx"
#endif


#endif //_VTKRenderMethodFactory_h
