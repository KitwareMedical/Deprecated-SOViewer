/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: OpenGLSliceTexture.h,v $
  Language:  C++
  Date:      $Date: 2006-05-05 20:57:46 $
  Version:   $Revision: 1.8 $
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __OpenGLSliceTexture_h_
#define __OpenGLSliceTexture_h_
#include <itkNumericTraits.h>
#include <itkRGBPixel.h>
#include <itkRGBAPixel.h>

#include "sovOpenGL.h"

#include "itkImage.h"
#include "itkConstantPadImageFilter.h"

namespace itk {

typedef RGBPixel<unsigned char>  RGBPixelType;
typedef RGBAPixel<unsigned char>  RGBAPixelType;

template <class T>
class NumericTraitsSOV : public NumericTraits<T>{};

template <>
class NumericTraitsSOV<RGBPixelType> : public NumericTraits<unsigned char>
{
public:
  typedef itk::RGBPixel<unsigned char> PrintType;

#if (ITK_VERSION_MAJOR <= 2 && ITK_VERSION_MINOR <= 6)
//  static const RGBPixel<unsigned char> ITKCommon_EXPORT Zero;
#endif
};

template <>
class NumericTraits<RGBAPixelType> : public NumericTraits<unsigned char>
{
public:
  typedef itk::RGBAPixel<unsigned char> PrintType;
#if (ITK_VERSION_MAJOR <= 2 && ITK_VERSION_MINOR <= 6)
// static const RGBAPixel<unsigned char> ITKCommon_EXPORT Zero;
#endif
};

}


/**
 * \class OpenGLSliceTexture
 * \brief This class is used to turn a 2D ITK image of (arbitrary) type
 * into a GL texture.  
 *
 * The calls to Update will make sure that the texture is up to date.  
 */
template<class TPixel> class OpenGLSliceTexture 
{
public:
  // Image typedefs
  typedef itk::Image<TPixel,2> ImageType;
  typedef typename ImageType::ConstPointer ImagePointer;

  /** Constructor, initializes the texture object */
  OpenGLSliceTexture();

  /** Destructor, deallocates texture memory */
  virtual ~OpenGLSliceTexture();
  
  /** Pass in a pointer to a 2D image */
  void SetImage(ImagePointer inImage);

  /** Get the type (e.g. GL_UNSIGNED_INT) in call to glTextureImage */
  void SetType(GLenum type) {m_GlType = type;}

  void SetFormat(GLenum type) {m_GlFormat = type;}

  /** Set the number of components */
  void SetComponents(GLuint num) {m_GlComponents = num;}

  /**
   * Make sure that the texture is up to date (reflects the image)
   */
  void Update();

  /**
   * Draw the texture in the current OpenGL context on a polygon with vertices
   * (0,0) - (size_x,size_y)
   */
  void Draw();

  /**
   * Draw the texture in transparent mode, with given level of alpha blending.
   */
  void DrawTransparent(unsigned char alpha);

  /**
   * Draw the texture in overlay mode, with given color and level of alpha blending.
   */
  void DrawOverlay(float r, float g, float b, float alpha);

  /** SetMinimum Value */
  void SetMininum(TPixel min) {m_Minimum = min;}

  /**  SetMaximumValue */
  void SetMaximum(TPixel max) {m_Maximum = max;}

  /** 
   * Set the orientation of the texture. The orientation of the texture is
   * affected when the user flip the image along one axis using any interactor
   * that offers such functionalities. 
   */
  void FlipX( bool flipX){ this->m_FlipX = flipX;}
  void FlipY( bool flipY){ this->m_FlipY = flipY;}

  bool FlipX(){return m_FlipX;}
  bool FlipY(){return m_FlipY;}

protected:

  bool m_FlipX;
  bool m_FlipY;
  bool m_FlipZ;

private:
  
  // Filter typedefs
  typedef itk::ConstantPadImageFilter<ImageType,ImageType> FilterType;
  typedef typename itk::SmartPointer<FilterType> FilterPointer;

  itk::Size<2> m_ImageSize;

  // The dimensions of the texture as stored in memory
  itk::Vector<unsigned int,2> m_TextureSize;
  
  // The texture offset between the edge of the image and the
  // boundary of the texture.
  vnl_vector_fixed<unsigned long,2> m_TextureOffset;

  // The pointer to the image from which the texture is computed
  ImagePointer m_Image;

  // The padding filter used to resize the image
  FilterPointer m_PadFilter;

  // The texture number (index)
  GLuint m_TextureIndex;

  // Has the texture been initialized?
  bool m_IsTextureInitalized;

  // The pipeline time of the source image (vs. our pipeline time)
  unsigned long m_UpdateTime;

  // The number of components for Gl op
  GLuint m_GlComponents;

  // The format for Gl op
  GLenum m_GlFormat;

  // The type for Gl op
  GLenum m_GlType;

  typename itk::NumericTraitsSOV<TPixel>::ValueType m_Maximum;
  typename itk::NumericTraitsSOV<TPixel>::ValueType m_Minimum;
};

#ifndef ITK_MANUAL_INSTANTIATION
#include "OpenGLSliceTexture.txx"
#endif

#endif // __OpenGLSliceTexture_h_
