/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovEllipseGlRenderMethod2D.h,v $
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
#ifndef sov_EllipseGlRenderMethod2D_h
#define sov_EllipseGlRenderMethod2D_h

#include "sovGlRenderMethod2D.h"
#include "itkEllipseSpatialObject.h"
#include "OpenGLSliceTexture.h"
#include "itkRGBPixel.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

class SOVOpenGLRenderMethod_EXPORT EllipseGlRenderMethod2D
: public GlRenderMethod2D
{

public:


  typedef EllipseGlRenderMethod2D        Self;
  typedef GlRenderMethod2D            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;
  typedef itk::RGBPixel<unsigned char> PixelType;
  typedef itk::Image<PixelType,2> ImageType;

  typedef OpenGLSliceTexture<PixelType> GreyTextureType;

  typedef itk::EllipseSpatialObject<2>           EllipseType;
  typedef EllipseType::Pointer                   EllipsePointer;

  itkNewMacro( EllipseGlRenderMethod2D );

  itkTypeMacro( EllipseGlRenderMethod2D, GlRenderMethod2D );

  virtual void draw( void );

protected:

  EllipseGlRenderMethod2D( void );
  ~EllipseGlRenderMethod2D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update(); 
  GreyTextureType* m_GreyTexture;
  ImageType::Pointer m_DisplaySliceImage;
};

}

#endif //_EllipseGlRenderMethod2D_h
