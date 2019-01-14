/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobGlRenderMethod2D.h,v $
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
#ifndef sov_BlobGlRenderMethod2D_h
#define sov_BlobGlRenderMethod2D_h

#include "sovGlRenderMethod2D.h"
#include "OpenGLSliceTexture.h"
#include "itkRGBAPixel.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

  
class SOVOpenGLRenderMethod_EXPORT BlobGlRenderMethod2D
: public GlRenderMethod2D
{

public:


  typedef BlobGlRenderMethod2D        Self;
  typedef GlRenderMethod2D            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;
  typedef itk::RGBAPixel<unsigned char> PixelType;
  typedef itk::Image<PixelType,2> ImageType;

  typedef OpenGLSliceTexture<PixelType> GreyTextureType;


  itkNewMacro( BlobGlRenderMethod2D );

  itkTypeMacro( BlobGlRenderMethod2D, GlRenderMethod2D );

  virtual void draw( void );

protected:

  BlobGlRenderMethod2D( void );
  ~BlobGlRenderMethod2D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;
  GreyTextureType* m_GreyTexture;
  virtual void Update(); 
  ImageType::Pointer m_DisplaySliceImage;
};

}

#endif //_BlobGlRenderMethod2D_h
