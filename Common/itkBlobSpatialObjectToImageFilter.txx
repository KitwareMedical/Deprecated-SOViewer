/*=========================================================================

Program:   itkUNC
Module:    $RCSfile: itkBlobSpatialObjectToImageFilter.txx,v $
Language:  C++
Date:      $Date: 2005-01-30 21:19:48 $
Version:   $Revision: 1.2 $
Author:    Julien Jomier

Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
See itkUNCCopyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkBlobSpatialObjectToImageFilter_txx
#define _itkBlobSpatialObjectToImageFilter_txx

#include "itkBlobSpatialObjectToImageFilter.h"
#include <vnl/vnl_vector.h>
#include "itkImageRegionIteratorWithIndex.h"
#include <itkSpatialObject.h>

namespace itk
{ 

/** Constructor */
template <unsigned int ObjectDimension,class TOutputImage>
BlobSpatialObjectToImageFilter<ObjectDimension,TOutputImage>
::BlobSpatialObjectToImageFilter()
{
  m_Size.Fill(0);
}

/** Destructor */
template <unsigned int ObjectDimension,class TOutputImage>
BlobSpatialObjectToImageFilter<ObjectDimension,TOutputImage>
::~BlobSpatialObjectToImageFilter()
{
}


/** Update */
template <unsigned int ObjectDimension,class TOutputImage>
void
BlobSpatialObjectToImageFilter<ObjectDimension,TOutputImage>
::GenerateData(void)
{
  itkDebugMacro(<< "BlobSpatialObjectToImageFilter::Update() called");

  //Get the input and output pointers 
  typename SpatialObject<ObjectDimension>::ConstPointer  
    InputBlob  = this->GetInput();
  typename SuperClass::OutputImagePointer      
    OutputImage = this->GetOutput();

  // Generate the image
  double origin[ObjectDimension];
  SizeType size;

  itk::Point<double,ObjectDimension> minimum;
  itk::Point<double,ObjectDimension> maximum;

  // We have to compute the bounding box in index space
  typename PointListType::const_iterator it  = static_cast<const BlobType*>(InputBlob.GetPointer())->GetPoints().begin();
  typename PointListType::const_iterator end = static_cast<const BlobType*>(InputBlob.GetPointer())->GetPoints().end();
  
  typedef typename SpatialObject<ObjectDimension>::BoundingBoxType BoundingBoxType;
  typename BoundingBoxType::Pointer m_Bounds = BoundingBoxType::New();
    
  if(it != end)
    {
    typename SpatialObject<ObjectDimension>::PointType pt = (*it).GetPosition();
    m_Bounds->SetMinimum(pt);
    m_Bounds->SetMaximum(pt);
    it++;
    while(it!= end) 
      {  
      typename SpatialObject<ObjectDimension>::PointType pt = (*it).GetPosition();
      m_Bounds->ConsiderPoint(pt);
      it++;
      }
    }
 
  for(unsigned int i=0;i<ObjectDimension;i++)
  {
    minimum[i] = m_Bounds->GetMinimum()[i];
    maximum[i] = m_Bounds->GetMaximum()[i];
  }
   
  unsigned long margin = 3;
  for(unsigned int i=0;i<ObjectDimension;i++)
  {
    size[i] = (unsigned long)(maximum[i] - minimum[i]+2*margin);
    origin[i] = (minimum[i]-margin)*InputBlob->GetSpacing()[i];
  }

  IndexType index;
  index.Fill(0);

  RegionType region;

  if(m_Size[0] == 0)
  {
    region.SetSize( size );
  }
  else
  {
    region.SetSize( m_Size );
  }
  region.SetIndex( index );

  OutputImage->SetLargestPossibleRegion( region); // 
  OutputImage->SetBufferedRegion( region );       // set the region 
  OutputImage->SetRequestedRegion( region );      //                                                                       
  OutputImage->SetSpacing(InputBlob->GetIndexToObjectTransform()->GetScaleComponent());   // set spacing
  OutputImage->SetOrigin(origin);   //   and origin
  OutputImage->Allocate();   // allocate the image  
  OutputImage->FillBuffer(0);

  typedef itk::BlobSpatialObject<ObjectDimension> myType;

  const PointListType pointList = ((myType *)&(*InputBlob))->GetPoints();  
  // get the list of pointss
  
  typename std::vector<ITK_TYPENAME myType::BlobPointType>::const_iterator
    pointIterator = pointList.begin();  
 
  while(pointIterator != pointList.end())
  {
    bool IsInside = true;
    for(unsigned int i=0; i<ObjectDimension; i++)
    {
      index[i] = (long unsigned int)((*pointIterator).GetPosition()[i]
                                     - minimum[i]+margin);
      if ((index[i]<0) ||
          (index[i]>= (int)(OutputImage->GetLargestPossibleRegion().GetSize()[i])))
      { 
        IsInside = false;
      }
    }

    if(IsInside)
    {
    OutputImage->SetPixel(index,1);
    }
    pointIterator++;
  }   

  itkDebugMacro(<< "BlobSpatialObjectToImageFilter::Update() finished");

} // end update function  


} // end namespace itk

#endif
