/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSparsePointsSpatialObjectPoint.h,v $
  Language:  C++
  Date:      $Date: 2005-07-21 16:22:20 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSparsePointsSpatialObjectPoint_h
#define __itkSparsePointsSpatialObjectPoint_h

#include "itkSpatialObjectPoint.h"
#include "itkVector.h"
#include "itkCovariantVector.h"
#include "itkArray.h"

namespace itk 
{

/** \class SparsePointsSpatialObjectPoint
* \brief Point used for a SparsePoints definition
*
* This class contains all the functions necessary to define a point
* that is a landmark, ridge, or surface point
* In 3D,
*   A landmark point has a position and three normals
*   A ridge point has a position and two normals
*   A surface point has a position and only one normal
*
* \sa SpatialObjectPoint 
*/ 
template < unsigned int TPointDimension = 3 >
class SparsePointsSpatialObjectPoint 
  : public SpatialObjectPoint<TPointDimension>
{

public:

  typedef SparsePointsSpatialObjectPoint           Self;
  typedef SpatialObjectPoint<TPointDimension>      Superclass;
  typedef Point< double, TPointDimension >         PointType;
  typedef Vector<double,TPointDimension>           VectorType;
  typedef CovariantVector<double,TPointDimension>  CovariantVectorType;
  typedef Vector<double,TPointDimension>           TangentType;
  typedef CovariantVector<double,TPointDimension>  NormalType;
  typedef double                                   WeightType;

  typedef std::vector<NormalType>                  NormalArrayType;
  typedef std::vector<TangentType>                 TangentArrayType;
  typedef std::vector<double>                      WeightArrayType;

  /** Constructor. This one defines the # of dimensions in the
   * SparsePointsSpatialObjectPoint */
  SparsePointsSpatialObjectPoint( void );

  SparsePointsSpatialObjectPoint( const Self & other );

  /** Default destructor. */
  virtual ~SparsePointsSpatialObjectPoint( void );

  /** The scale at which the point is well defined. e.g., tubePoint = radius */
  void SetScale( double scale );

  /** The scale at which the point is well defined, e.g., tubePoint = radius */
  double GetScale( void ) const;

  /** The weight of importance/validity/fit of the point */
  void SetWeight( double weight );

  /** The weight of importance/validity/fit of the point */
  double GetWeight( void ) const;

  /** The number of normal directions expected for this point.
   * Also sets number of tangents as NumDims - numNormals */
  void SetNumberOfNormals( unsigned int nNormals );

  /** The number of normal directions expected for this point */
  unsigned int GetNumberOfNormals( void ) const;

  /** Set Normal */
  void SetNormal(unsigned int normNum, const NormalType & normal);
  void SetNormal(unsigned int normNum, double normalx, double normaly);
  void SetNormal(unsigned int normNum, double normalx, double normaly,
                                       double normalz);

  /** Get Normals */
  const NormalType & GetNormal( unsigned int nNum = 0 ) const;

  /** Set the relative weight (importance/strength/confidence) associated
   * with a normal direction */
  void SetNormalWeight( unsigned int normNum, double nWeight );

  /** Get the relative weight (importance/strength/confidence) associated
   * with a normal direction */
  double GetNormalWeight( unsigned int nNum = 0 ) const;


  /** The number of tangent directions expected for this point */
  void SetNumberOfTangents( unsigned int nTangents );

  /** The number of tangent directions expected for this point 
   * Also sets number of normals as NumDims - numTangents */
  unsigned int GetNumberOfTangents( void ) const;

  /** Set Tangent */
  void SetTangent(unsigned int tangNum, const TangentType & tangent);
  void SetTangent(unsigned int tangNum, double tangentx, double tangenty);
  void SetTangent(unsigned int tangNum, double tangentx, double tangenty,
                                        double tangentz);

  /** Get Tangents */
  const VectorType & GetTangent( unsigned int tNum = 0 ) const;

  /** Set the relative weight (importance/strength/confidence) associated
   * with a tangent direction */
  void SetTangentWeight( unsigned int tNum, double tWeight );

  /** Get the relative weight (importance/strength/confidence) associated
   * with a tangent direction */
  double GetTangentWeight( unsigned int tNum = 0 ) const;

  /** Copy one SparsePointsSpatialObjectPoint to another */
  Self & operator=(const SparsePointsSpatialObjectPoint & rhs);

  /** Set/Get the displacement vector */
  void SetDisplacementVector(const VectorType & v) {m_DisplacementVector = v;}
  const VectorType & GetDisplacementVector(void) const { return m_DisplacementVector;}

protected:

  double        m_Scale;
  double        m_Weight;

  unsigned int         m_NumberOfNormals;
  NormalArrayType      m_Normal;
  WeightArrayType      m_NormalWeight;

  unsigned int         m_NumberOfTangents;
  TangentArrayType     m_Tangent;
  WeightArrayType      m_TangentWeight;
  VectorType           m_DisplacementVector;
  
  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;
};

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSparsePointsSpatialObjectPoint.txx"
#endif

#endif // __itkSparsePointsSpatialObjectPoint_h
