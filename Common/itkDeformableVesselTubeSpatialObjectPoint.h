/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDeformableVesselTubeSpatialObjectPoint.h,v $
  Language:  C++
  Date:      $Date: 2005-03-13 22:24:43 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkDeformableVesselTubeSpatialObjectPoint_h
#define __itkDeformableVesselTubeSpatialObjectPoint_h

#include "itkVesselTubeSpatialObjectPoint.h"
#include "itkCovariantVector.h"
#include "vnl/vnl_vector_fixed.h"

namespace itk 
{

/** \class DeformableVesselTubeSpatialObjectPoint
* \brief Point used for a tube definition
*
* This class contains all the functions necessary to define a point
* that can be used to build tubes.
*
* \sa DeformableVesselTubeSpatialObject 
*/ 
template < unsigned int TPointDimension = 3 >
class DeformableVesselTubeSpatialObjectPoint 
  : public VesselTubeSpatialObjectPoint<TPointDimension>
{

public:

  typedef DeformableVesselTubeSpatialObjectPoint                 Self;
  typedef VesselTubeSpatialObjectPoint<TPointDimension>      Superclass;
  typedef Point< double, TPointDimension >             PointType;
  typedef Vector<double, TPointDimension >             VectorType;
  typedef CovariantVector<double, TPointDimension >    CovariantVectorType;
 
  /** Constructor. This one defines the # of dimensions in the 
   * DeformableVesselTubeSpatialObjectPoint */
  DeformableVesselTubeSpatialObjectPoint( void );

  /** Default destructor. */
  virtual ~DeformableVesselTubeSpatialObjectPoint( void );

  /** Copy one DeformableVesselTubeSpatialObjectPoint to another */
  Self & operator=(const DeformableVesselTubeSpatialObjectPoint & rhs);

  /** Set/Get the original tangent */
  void SetOriginalTangent(const VectorType & newT);
  VectorType GetOriginalTangent() {return m_OriginalTangent;}

  /** Set/Get the original distance from previous point */
  void SetOriginalDistance(double distance);
  double GetOriginalDistance() {return m_OriginalDistance;}
  
  /** Set the displacement */
  void SetDisplacement(const VectorType & displacement);
  const VectorType GetDisplacement() const {return m_Displacement;}

protected:

  /** Print the object */
  void PrintSelf( std::ostream & os, Indent indent) const;

  VectorType m_OriginalTangent;
  VectorType m_Displacement;
  double m_OriginalDistance; //distance from previous point

};

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDeformableVesselTubeSpatialObjectPoint.txx"
#endif

#endif // __itkDeformableVesselTubeSpatialObjectPoint_h
