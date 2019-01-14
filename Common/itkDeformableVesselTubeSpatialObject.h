/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDeformableVesselTubeSpatialObject.h,v $
  Language:  C++
  Date:      $Date: 2005-03-13 22:24:43 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDeformableVesselTubeSpatialObject_h
#define __itkDeformableVesselTubeSpatialObject_h

#include <list>

#include "itkVesselTubeSpatialObject.h"
#include "itkDeformableVesselTubeSpatialObjectPoint.h"

namespace itk 
{

/** 
* \class DeformableVesselTubeSpatialObject
* \brief Representation of a tube based on the spatial object classes.
*
* The tube is basically defined by a set of points. Each tube can
* be connected to a tube network, by using the AddSpatialObject() methods
* of a DeformableVesselTubeSpatialObject Object. A tube is also identified by an id number when connected
* to a network.
*
* \sa DeformableVesselTubeSpatialObjectPoint 
*/

template < unsigned int TDimension = 3 >
class DeformableVesselTubeSpatialObject
  :public TubeSpatialObject< TDimension, DeformableVesselTubeSpatialObjectPoint< TDimension >  >
{

public:

  typedef DeformableVesselTubeSpatialObject                      Self;
  typedef TubeSpatialObject< TDimension,
          DeformableVesselTubeSpatialObjectPoint< TDimension > > Superclass;
  typedef SmartPointer < Self >                        Pointer;
  typedef SmartPointer < const Self >                  ConstPointer;
  typedef DeformableVesselTubeSpatialObjectPoint< TDimension >   TubePointType;
  typedef typename Superclass::PointListType           PointListType;
  typedef typename Superclass::PointType               PointType;
  typedef typename Superclass::TransformType           TransformType;
  typedef typename Superclass::SpatialObjectPointType  SpatialObjectPointType;
  typedef VectorContainer<unsigned long,PointType>     PointContainerType;
  typedef SmartPointer<PointContainerType>             PointContainerPointer;
  typedef typename Superclass::VectorType              VectorType;
  typedef typename Superclass::CovariantVectorType     CovariantVectorType;
  typedef typename Superclass::BoundingBoxType         BoundingBoxType;
  typedef VesselTubeSpatialObject<TDimension>          VesselTubeSpatialObjectType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Method for creation through the object factory. */
  itkTypeMacro( DeformableVesselTubeSpatialObject, TubeSpatialObject );

  /** Fill in the DeformableVesselTubeSpatialObject given a VesselTubeSpatialObject */
  void SetVesselTubeSpatialObject(VesselTubeSpatialObjectType* vessel);
    
protected:

  DeformableVesselTubeSpatialObject();
  virtual ~DeformableVesselTubeSpatialObject();

  /** Method to print the object.*/
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;
  
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION 
#include "itkDeformableVesselTubeSpatialObject.txx" 
#endif 

#endif // __itkDeformableVesselTubeSpatialObject_h
