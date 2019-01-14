/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:  $RCSfile: itkSparsePointsSpatialObject.h,v $
  Language:  C++
  Date:    $Date: 2005-04-07 16:58:56 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

   This software is distributed WITHOUT ANY WARRANTY; without even 
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
   PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSparsePointsSpatialObject_h
#define __itkSparsePointsSpatialObject_h

#include <list>

#include "itkPointBasedSpatialObject.h"
#include "itkSparsePointsSpatialObjectPoint.h"

namespace itk 
{

/** 
* \class itkSparsePointsSpatialObject
* \brief Representation of a collection of points from physical objects having
* an arbitrary numbers of normal and tangent vectors (e.g., landmark in 3D has
* 3 normals and no tangents, a point on a surface in 3D has 1 normal and 2
* tangets, etc).  This class is based on the point based spatial object classes.
*
* \sa SparsePointsSpatialObjectPoint
*/

template < unsigned int TDimension = 3 >
class SparsePointsSpatialObject 
  :public PointBasedSpatialObject<  TDimension >
{

public:

  typedef SparsePointsSpatialObject                     Self;
  typedef PointBasedSpatialObject< TDimension >         Superclass;
  typedef SmartPointer < Self >                         Pointer;
  typedef SmartPointer < const Self >                   ConstPointer;
  typedef double                                        ScalarType;
  typedef SparsePointsSpatialObjectPoint< TDimension >  SparsePointType;
  typedef std::vector< SparsePointType >                SparsePointListType;
  typedef typename Superclass::SpatialObjectPointType   SpatialObjectPointType;
  typedef typename Superclass::PointType                PointType;
  typedef typename Superclass::TransformType            TransformType;
  typedef VectorContainer<unsigned long,PointType>      PointContainerType;
  typedef SmartPointer<PointContainerType>              PointContainerPointer;
  typedef typename Superclass::BoundingBoxType          BoundingBoxType;

  /** Method for creation through the object factory.*/
  itkNewMacro( Self );

  /** Method for creation through the object factory. */
  itkTypeMacro( Self, Superclass );
  
  /** Returns a reference to the list of the points. */
  SparsePointListType & GetPoints( void );

  /** Returns a reference to the list of the points. */
  const SparsePointListType & GetPoints( void ) const;

  /** Return a point in the list given the index */
  const SpatialObjectPointType* GetPoint(unsigned long id) const
        {return &(m_Points[id]);}

  /** Return a point in the list given the index */
  SpatialObjectPointType* GetPoint(unsigned long id) {return &(m_Points[id]);}
  
  /** Return the number of points in the list */
  unsigned long GetNumberOfPoints(void) const {return m_Points.size();}

  /** Set the list of points. */
  void SetPoints( SparsePointListType & newPoints );

  /** Returns true if the list of points is evaluable at the requested point, 
   * false otherwise. */
  bool IsEvaluableAt( const PointType & point,
                      unsigned int depth=0, char * name=NULL) const;

  /** Returns the value of the list of points at that point.
   *  Currently this function returns a binary value,
   *  but it might want to return a degree of membership
   *  in case of fuzzy Surfaces. */
  bool ValueAt( const PointType & point, double & value,
                unsigned int depth=0, char * name=NULL) const;

  /** Returns true if the point is in the list, false otherwise. */
  bool IsInside( const PointType & point,
                 unsigned int depth, char * name) const;

  /** Test whether a point is inside or outside the object 
   *  For computational speed purposes, it is faster if the method does not
   *  check the name of the class and the current depth */ 
  virtual bool IsInside( const PointType & point) const;

  /** Compute the boundaries of the Surface. */
  bool ComputeLocalBoundingBox( ) const;

  /** Compute the center of rotation for this collection of points */
  const PointType GetCenter( ) const;

protected:
  SparsePointsSpatialObject();
  virtual ~SparsePointsSpatialObject();

  /** Method to print the object.*/
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

  SparsePointListType  m_Points;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION 
#include "itkSparsePointsSpatialObject.txx" 
#endif 

#endif // __itkSparsePointsSpatialObject_h
