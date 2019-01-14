/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSparsePointsSpatialObject.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-09 15:37:03 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifndef __itkSparsePointsSpatialObject_txx
#define __itkSparsePointsSpatialObject_txx

#include "itkSparsePointsSpatialObject.h" 

namespace itk  
{ 

/** Constructor */
template< unsigned int TDimension >
SparsePointsSpatialObject< TDimension > 
::SparsePointsSpatialObject()  
{ 
  this->SetDimension(TDimension);
  this->SetTypeName("SparsePointsSpatialObject");
  this->GetProperty()->SetRed(1); 
  this->GetProperty()->SetGreen(0); 
  this->GetProperty()->SetBlue(0); 
  this->GetProperty()->SetAlpha(1); 
  this->ComputeBoundingBox();
} 

/** Destructor */ 
template< unsigned int TDimension >
SparsePointsSpatialObject< TDimension >  
::~SparsePointsSpatialObject()
{ 
} 
 
/** Get the list of points composing the surface */
template< unsigned int TDimension >
typename SparsePointsSpatialObject< TDimension >::SparsePointListType &  
SparsePointsSpatialObject< TDimension > 
::GetPoints() 
{ 
  itkDebugMacro( "Getting SparsePointsPoint list" );
  return m_Points;
} 
 
/** Get the list of points composing the surface */
template< unsigned int TDimension >
const typename SparsePointsSpatialObject< TDimension >::SparsePointListType &  
SparsePointsSpatialObject< TDimension > 
::GetPoints() const
{ 
  itkDebugMacro( "Getting SparsePointsPoint list" );
  return m_Points;
} 
 
/** Set the list of points composing the surface */
template< unsigned int TDimension >
void  
SparsePointsSpatialObject< TDimension >  
::SetPoints( SparsePointListType & points )  
{
  // in this function, passing a null pointer as argument will
  // just clear the list...
  m_Points.clear();
   
  typename SparsePointListType::iterator it,end;
  it = points.begin();    
  end = points.end();
  while(it != end)
    {
    m_Points.push_back(*it);

    it++;
    } 

  this->Modified();
}
 
/** Set the list of points composing the surface */
template< unsigned int TDimension >
const typename SparsePointsSpatialObject< TDimension >::PointType 
SparsePointsSpatialObject< TDimension >  
::GetCenter( ) const
{
  PointType center;

  center.Fill(0);
  unsigned int count = 0;

  typename SparsePointListType::const_iterator it,end;
  it = m_Points.begin();    
  end = m_Points.end();
  while(it != end)
    {
    for(unsigned int i=0; i<TDimension; i++)
      {
      center[i] = center[i] + it->GetPosition()[i];
      }
    count++;

    it++;
    } 


  if( count != 0 )
    {
    for(unsigned int i=0; i<TDimension; i++)
      {
      center[i] = center[i] / count; 
      }
    }

  return center;
}
 
/** Print the surface object */
template< unsigned int TDimension >
void  
SparsePointsSpatialObject< TDimension >  
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "SparsePointsSpatialObject(" << this << ")" << std::endl; 
  os << indent << "ID: " << this->GetId() << std::endl; 
  os << indent << "nb of points: "
               << static_cast<unsigned long>( m_Points.size() ) << std::endl;

  Superclass::PrintSelf( os, indent ); 
} 

/** Compute the bounds of the surface */
template< unsigned int TDimension >
bool 
SparsePointsSpatialObject< TDimension >  
::ComputeLocalBoundingBox() const
{ 
  itkDebugMacro( "Computing surface bounding box" );

  if( this->GetBoundingBoxChildrenName().empty() 
      || strstr(typeid(Self).name(),
                this->GetBoundingBoxChildrenName().c_str()) )
    {
    typename SparsePointListType::const_iterator it  = m_Points.begin();
    typename SparsePointListType::const_iterator end = m_Points.end();
  
    if(it == end)
      {
      return false;
      }
    else
      {
      PointType pt = this->GetIndexToWorldTransform()->
                           TransformPoint((*it).GetPosition());
      const_cast<BoundingBoxType *>(this->GetBounds())->SetMinimum(pt);
      const_cast<BoundingBoxType *>(this->GetBounds())->SetMaximum(pt);
      it++;
      while(it!= end) 
        {
        pt = this->GetIndexToWorldTransform()->
                             TransformPoint((*it).GetPosition());
        const_cast<BoundingBoxType *>(this->GetBounds())->ConsiderPoint(pt);
        it++;
        }
      }
    }
  return true;
} 

/** Test whether a point is inside or outside the object 
 *  For computational speed purposes, it is faster if the method does not
 *  check the name of the class and the current depth */ 
template< unsigned int TDimension >
bool 
SparsePointsSpatialObject< TDimension >
::IsInside( const PointType & point) const
{ 
  typename SparsePointListType::const_iterator it = m_Points.begin();
  typename SparsePointListType::const_iterator itEnd = m_Points.end();
    
  if(!this->GetIndexToWorldTransform()->
            GetInverse(const_cast<TransformType *>
                       (this->GetInternalInverseTransform())))
    {
    return false;
    }

  PointType transformedPoint = this->GetInternalInverseTransform()->
                                     TransformPoint(point);
  
  if( this->GetBounds()->IsInside(transformedPoint) )
    {
    while(it != itEnd)
      {
      if((*it).GetPosition() == transformedPoint)
        {
        return true;
        }
      it++;
      }
    }
  return false;
}


/** Return true is the given point is on the surface */
template< unsigned int TDimension >
bool 
SparsePointsSpatialObject< TDimension >  
::IsInside( const PointType & point, unsigned int depth, char * name ) const
{
  itkDebugMacro( "Checking the point [" << point << "is on the surface" );

  if(name == NULL)
    {
    if(IsInside(point))
      {
      return true;
      }
    }
  else if(strstr(typeid(Self).name(), name))
    {
    if(IsInside(point))
      {
      return true;
      }
    }
  
  return Superclass::IsInside(point, depth, name);
} 

/** Return true if the surface is evaluable at a specified point */
template< unsigned int TDimension >
bool
SparsePointsSpatialObject< TDimension > 
::IsEvaluableAt( const PointType & point, unsigned int depth, char * name )
  const
{
  itkDebugMacro( "Checking if the surface is evaluable at " << point );
  return IsInside(point, depth, name);
}

/** Return 1 if the point is on the surface */
template< unsigned int TDimension >
bool
SparsePointsSpatialObject< TDimension > 
::ValueAt( const PointType & point, double & value, unsigned int depth,
           char * name ) const
{
  itkDebugMacro( "Getting the value of the surface at " << point );
  if( IsInside(point, 0, name) )
    {
    value = 1;
    return true;
    }
  else
    {
    if( Superclass::IsEvaluableAt(point, depth, name) )
      {
      Superclass::ValueAt(point, value, depth, name);
      return true;
      }
    else
      {
      value = 0;
      return false;
      }
    }
  return false;
}

} // end namespace itk 

#endif
