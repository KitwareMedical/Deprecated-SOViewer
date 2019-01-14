/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDeformableVesselTubeSpatialObjectPoint.txx,v $
  Language:  C++
  Date:      $Date: 2005-04-06 21:35:11 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDeformableVesselTubeSpatialObjectPoint_txx
#define __itkDeformableVesselTubeSpatialObjectPoint_txx

#include "itkDeformableVesselTubeSpatialObjectPoint.h"

namespace itk 
{

/** Constructor */
template< unsigned int TPointDimension >
DeformableVesselTubeSpatialObjectPoint< TPointDimension >
::DeformableVesselTubeSpatialObjectPoint( void ) : Superclass()
{ 
  m_OriginalTangent.Fill(0);
  m_Displacement.Fill(0);
  m_OriginalDistance = 0;
}

/** Destructor */
template< unsigned int TPointDimension >
DeformableVesselTubeSpatialObjectPoint< TPointDimension >
::~DeformableVesselTubeSpatialObjectPoint( void ) 
{
}


/** Set the original tangent */
template< unsigned int TPointDimension >
void 
DeformableVesselTubeSpatialObjectPoint< TPointDimension >
::SetOriginalTangent( const VectorType & newT ) 
{
  m_OriginalTangent = newT;
}

/** Set the displacement */
template< unsigned int TPointDimension >
void 
DeformableVesselTubeSpatialObjectPoint< TPointDimension >
::SetDisplacement( const VectorType & displacement ) 
{
  m_Displacement = displacement;
}

template< unsigned int TPointDimension >
void 
DeformableVesselTubeSpatialObjectPoint< TPointDimension >
::SetOriginalDistance( double distance ) 
{
  m_OriginalDistance = distance;
}

template< unsigned int TPointDimension >
void
DeformableVesselTubeSpatialObjectPoint< TPointDimension >
::PrintSelf( std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "OriginalTangent: " << m_OriginalTangent << std::endl;
  os << indent << "Displacement: " << m_Displacement << std::endl;
  os << indent << "OriginalDistance: " << m_OriginalDistance << std::endl;
}

template< unsigned int TPointDimension >
typename DeformableVesselTubeSpatialObjectPoint< TPointDimension >::Self & 
DeformableVesselTubeSpatialObjectPoint< TPointDimension >
::operator=(const DeformableVesselTubeSpatialObjectPoint & rhs) 
{
  this->m_ID = rhs.m_ID;
  this->m_R = rhs.m_R;
  this->m_Medialness = rhs.m_Medialness;
  this->m_Ridgeness = rhs.m_Ridgeness;
  this->m_Branchness = rhs.m_Branchness;
  this->m_Mark = rhs.m_Mark;
  this->m_NumDimensions = rhs.m_NumDimensions;
  this->m_X = rhs.m_X;
  this->m_T = rhs.m_T;
  this->m_Normal1 = rhs.m_Normal1;
  this->m_Normal2 = rhs.m_Normal2;
  this->m_Alpha1 = rhs.m_Alpha1;
  this->m_Alpha2 = rhs.m_Alpha2;
  this->m_Alpha3 = rhs.m_Alpha3;
  this->m_Color = rhs.m_Color;
  this->m_OriginalTangent = rhs.m_OriginalTangent;
  this->m_Displacement = rhs.m_Displacement;
  this->m_OriginalDistance = rhs.m_OriginalDistance;
  return * this;
}

} // end namespace itk

#endif
