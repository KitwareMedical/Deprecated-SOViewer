/*=========================================================================

  Program:   SOViewer
  Module:    $RCSfile: itkBlobSpatialObjectToImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-12-08 15:18:22 $
  Version:   $Revision: 1.14 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBlobSpatialObjectToImageFilter_h
#define __itkBlobSpatialObjectToImageFilter_h

#include <itkSpatialObjectToImageFilter.h>
#include <itkBlobSpatialObject.h>
#include <itkSpatialObjectPoint.h>
#include <itkSpatialObject.h>



namespace itk
{
/** \class itkBlobSpatialObjectToImageFilter
 * this filter create a binary image, whom his type is
 * the value templated, from a blob spatial object */
template <unsigned int NObjectDimension, class TOutputImage>
class ITK_EXPORT BlobSpatialObjectToImageFilter :   
    public SpatialObjectToImageFilter< SpatialObject<NObjectDimension>,
                                       TOutputImage>
{
public:

  /** Standard class typedefs. */
  typedef BlobSpatialObjectToImageFilter<
    NObjectDimension,
    TOutputImage>                                        Self;
  typedef SpatialObjectToImageFilter<
    SpatialObject<NObjectDimension>, TOutputImage>       SuperClass;
  typedef SmartPointer<Self>                             Pointer;
  typedef SmartPointer<const Self>                       ConstPointer;

  typedef BlobSpatialObject<NObjectDimension>     BlobType;
  typedef typename BlobType::PointListType        PointListType;
  typedef typename TOutputImage::SizeType         SizeType;
  typedef typename TOutputImage::IndexType        IndexType;
  typedef typename TOutputImage::RegionType       RegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(BlobSpatialObjectToImageFilter, SpatialObjectToImageFilter);

  /** Create the output images and fill it */
  void GenerateData();

  /** Set Size */
  itkSetMacro(Size,SizeType);


protected:
  BlobSpatialObjectToImageFilter();
  ~BlobSpatialObjectToImageFilter();

private:

  SizeType          m_Size;

  void PrintSelf(std::ostream& os, Indent indent) const
  { SuperClass::PrintSelf(os,indent);   }


}; // end of class 

}// end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBlobSpatialObjectToImageFilter.txx"
#endif   // ITK_MANUAL_INSTANTIATION

#endif  // _itkBlobSpatialObjectToImageFilter_h
