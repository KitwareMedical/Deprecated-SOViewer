/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDeformableVesselTubeSpatialObject.txx,v $
  Language:  C++
  Date:      $Date: 2005-04-06 21:35:11 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifndef __itkDeformableVesselTubeSpatialObject_txx
#define __itkDeformableVesselTubeSpatialObject_txx

#include "itkDeformableVesselTubeSpatialObject.h" 

namespace itk  
{ 

/** Constructor */
template< unsigned int TDimension >
DeformableVesselTubeSpatialObject< TDimension > 
::DeformableVesselTubeSpatialObject() : Superclass()
{ 
  this->m_ParentPoint = -1;
  this->SetDimension(TDimension);
  // This should be DeformableVesselTubeSpatialObject but for SOVfactory
  // we do the hack for the moment :)
  this->SetTypeName("DeformableVesselTubeSpatialObject");
} 
 
/** Destructor */
template< unsigned int TDimension >
DeformableVesselTubeSpatialObject< TDimension >  
::~DeformableVesselTubeSpatialObject()
{ 
} 

/** Print the object */ 
template< unsigned int TDimension >
void  
DeformableVesselTubeSpatialObject< TDimension >  
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "DeformableVesselTubeSpatialObject(" << this << ")" << std::endl; 
  Superclass::PrintSelf( os, indent ); 
}

/** Fill in the DeformableVesselTubeSpatialObject given a VesselTubeSpatialObject */
template< unsigned int TDimension >
void  
DeformableVesselTubeSpatialObject< TDimension >  
::SetVesselTubeSpatialObject(VesselTubeSpatialObjectType* vessel)
{
  this->m_ParentPoint = vessel->GetParentPoint();
  this->m_EndType = vessel->GetEndType();
  this->GetProperty()->SetColor(vessel->GetProperty()->GetColor());
  this->SetId(vessel->GetId());
  this->SetSpacing(vessel->GetSpacing());

  // Now copy the points
  typename VesselTubeSpatialObjectType::PointListType vesselPoints = vessel->GetPoints();
  typename VesselTubeSpatialObjectType::PointListType::const_iterator it = vesselPoints.begin();
  while(it != vesselPoints.end())
    {
    TubePointType pt;
    // do the copy
    pt.SetPosition((*it).GetPosition());
    pt.SetRadius((*it).GetRadius());
    pt.SetTangent((*it).GetTangent());
    pt.SetNormal1((*it).GetNormal1());
    pt.SetNormal2((*it).GetNormal2());
    pt.SetAlpha1((*it).GetAlpha1());
    pt.SetAlpha2((*it).GetAlpha2());
    pt.SetAlpha3((*it).GetAlpha3());
    pt.SetMedialness((*it).GetMedialness());
    pt.SetRidgeness((*it).GetRidgeness());
    pt.SetBranchness((*it).GetBranchness());
    pt.SetMark((*it).GetMark());
    pt.SetColor((*it).GetColor());
    this->m_Points.push_back(pt);
    it++;
    }

}


} // end namespace itk 

#endif // end __itkDeformableVesselTubeSpatialObject_txx
