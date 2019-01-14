/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSparsePointsSpatialObjectPoint.txx,v $
  Language:  C++
  Date:      $Date: 2005-07-21 16:22:20 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSparsePointsSpatialObjectPoint_txx
#define __itkSparsePointsSpatialObjectPoint_txx

#include "itkSparsePointsSpatialObjectPoint.h"
#include "itkExceptionObject.h"

namespace itk 
{

/** Constructor */
template< unsigned int TPointDimension >
SparsePointsSpatialObjectPoint< TPointDimension >
::SparsePointsSpatialObjectPoint( void ) 
{ 
  m_Scale = 1;
  m_Weight = 1;

  m_NumberOfNormals = 0;
  m_NumberOfTangents = 0;
  m_DisplacementVector.Fill(0);
  NormalType n;
  TangentType t;
  for(int i=0; i<TPointDimension; i++)
    {
    m_Normal.push_back(n);
    m_NormalWeight.push_back(0.0);
    m_Tangent.push_back(t);
    m_TangentWeight.push_back(0.0);
    }

}

/** Constructor */
template< unsigned int TPointDimension >
SparsePointsSpatialObjectPoint< TPointDimension >
::SparsePointsSpatialObjectPoint( const Self & other ) : Superclass( other )
{ 
  m_Scale = other.GetScale();
  m_Weight = other.GetWeight();

  NormalType n;
  TangentType t;
  for(int i=0; i<TPointDimension; i++)
    {
    m_Normal.push_back(n);
    m_NormalWeight.push_back(0.0);
    m_Tangent.push_back(t);
    m_TangentWeight.push_back(0.0);
    }

  m_NumberOfNormals = other.GetNumberOfNormals();
  for(int i=0; i<m_NumberOfNormals; i++)
    {
    m_Normal[i] = other.GetNormal(i);
    m_NormalWeight[i] = other.GetNormalWeight(i);
    }

  m_NumberOfTangents = other.GetNumberOfTangents();
  for(int i=0; i<m_NumberOfTangents; i++)
    {
    m_Tangent[i] = other.GetTangent(i);
    m_TangentWeight[i] = other.GetTangentWeight(i);
    }

  m_DisplacementVector = other.GetDisplacementVector();
}

/** Destructor */
template< unsigned int TPointDimension >
SparsePointsSpatialObjectPoint< TPointDimension >
::~SparsePointsSpatialObjectPoint( void ) 
{
}

/** Set the scale for this point */
template< unsigned int TPointDimension >
void
SparsePointsSpatialObjectPoint< TPointDimension >
::SetScale( double scale )
{
  m_Scale = scale;
}

/** Get the scale for this point */
template< unsigned int TPointDimension >
double
SparsePointsSpatialObjectPoint< TPointDimension >
::GetScale( void ) const
{
  return m_Scale;
}

/** Set the weight/importance for this point */
template< unsigned int TPointDimension >
void
SparsePointsSpatialObjectPoint< TPointDimension >
::SetWeight( double weight )
{
  m_Weight = weight;
}

/** Get the weight/importance for this point */
template< unsigned int TPointDimension >
double
SparsePointsSpatialObjectPoint< TPointDimension >
::GetWeight( void ) const
{
  return m_Weight;
}

/** Set the number of normal directions expected for this point */
template< unsigned int TPointDimension >
void
SparsePointsSpatialObjectPoint< TPointDimension >
::SetNumberOfNormals( unsigned int nNormals )
{

  for(int i=nNormals; i<m_NumberOfNormals; i++)
    {
    m_Normal[i].Fill(0);
    m_NormalWeight[i] = 0;
    }
  m_NumberOfNormals = nNormals;

  int nTangents = TPointDimension - nNormals;
  m_NumberOfTangents = m_Tangent.size();
  for(int i=nTangents; i<m_NumberOfTangents; i++)
    {
    m_Tangent[i].Fill(0);
    m_TangentWeight[i] = 0;
    }
  m_NumberOfTangents = nTangents;

}

/** Set the normal : N-D case */
template< unsigned int TPointDimension >
unsigned int 
SparsePointsSpatialObjectPoint< TPointDimension >
::GetNumberOfNormals( void ) const
{
  return m_NumberOfNormals;
}

/** Set the normal : N-D case */
template< unsigned int TPointDimension >
void 
SparsePointsSpatialObjectPoint< TPointDimension >
::SetNormal( unsigned int normN, const NormalType & normal ) 
{
  if(normN >= m_NumberOfNormals)
    {
    this->SetNumberOfNormals(normN+1);
    }
  m_Normal[normN] = normal;
}

/** Set the normal : 2D case */
template< unsigned int TPointDimension >
void 
SparsePointsSpatialObjectPoint< TPointDimension >
::SetNormal( unsigned int normN, double normalx, double normaly)
{
  if(normN >= m_NumberOfNormals)
    {
    this->SetNumberOfNormals(normN+1);
    }
  m_Normal[normN][0] = normalx;
  m_Normal[normN][1] = normaly;
}

/** Set the normal : 3D case */
template< unsigned int TPointDimension >
void 
SparsePointsSpatialObjectPoint< TPointDimension >
::SetNormal( unsigned int normN, double normalx, double normaly, double normalz)
{
  if(normN >= m_NumberOfNormals)
    {
    this->SetNumberOfNormals(normN+1);
    }
  m_Normal[normN][0] = normalx;
  m_Normal[normN][1] = normaly;
  m_Normal[normN][2] = normalz;
}

/** Get the normal at one point */
template< unsigned int TPointDimension >
const typename SparsePointsSpatialObjectPoint< TPointDimension >::NormalType &
SparsePointsSpatialObjectPoint< TPointDimension >
::GetNormal( unsigned int normN ) const
{
  if(normN >= m_NumberOfNormals)
    {
    ExceptionObject exception(__FILE__, __LINE__);
    exception.SetDescription("Requested a non-existent normal direction");
    throw exception;
    }
  return m_Normal[normN];
}
  
/** Set the weight of the normal at a point */
template< unsigned int TPointDimension >
void
SparsePointsSpatialObjectPoint< TPointDimension >
::SetNormalWeight( unsigned int normN, double nWeight ) 
{
  if(normN >= m_NumberOfNormals)
    {
    ExceptionObject exception(__FILE__, __LINE__);
    exception.SetDescription("Tried to set weight of a non-existent normal");
    throw exception;
    }
  m_NormalWeight[normN] = nWeight;
}
  
/** Get the weight of the normal at a point */
template< unsigned int TPointDimension >
double
SparsePointsSpatialObjectPoint< TPointDimension >
::GetNormalWeight( unsigned int normN ) const
{
  if(normN >= m_NumberOfNormals)
    {
    ExceptionObject exception(__FILE__, __LINE__);
    exception.SetDescription("Tried to get weight of a non-existent normal");
    throw exception;
    }
  return m_NormalWeight[normN];
}

/** Set the number of tangent directions expected for this point */
template< unsigned int TPointDimension >
void
SparsePointsSpatialObjectPoint< TPointDimension >
::SetNumberOfTangents( unsigned int nTangents )
{
  this->SetNumberOfNormals(TPointDimension-nTangents);
}

/** Get the number of tangent directions expected for this point */
template< unsigned int TPointDimension >
unsigned int 
SparsePointsSpatialObjectPoint< TPointDimension >
::GetNumberOfTangents( void ) const
{
  return m_NumberOfTangents;
}

/** Set the tangent : N-D case */
template< unsigned int TPointDimension >
void 
SparsePointsSpatialObjectPoint< TPointDimension >
::SetTangent( unsigned int tangN, const TangentType & tangent ) 
{
  if(tangN >= m_NumberOfTangents)
    {
    this->SetNumberOfTangents(tangN+1);
    }
  m_Tangent[tangN] = tangent;
}

/** Set the tangent : 2D case */
template< unsigned int TPointDimension >
void 
SparsePointsSpatialObjectPoint< TPointDimension >
::SetTangent( unsigned int tangN, double tangentx, double tangenty)
{
  if(tangN >= m_NumberOfTangents)
    {
    this->SetNumberOfTangents(tangN+1);
    }
  m_Tangent[tangN][0] = tangentx;
  m_Tangent[tangN][1] = tangenty;
}

/** Set the tangent : 3D case */
template< unsigned int TPointDimension >
void 
SparsePointsSpatialObjectPoint< TPointDimension >
::SetTangent( unsigned int tangN, double tangentx, double tangenty,
                                  double tangentz)
{
  if(tangN >= m_NumberOfTangents)
    {
    this->SetNumberOfTangents(tangN+1);
    }
  m_Tangent[tangN][0] = tangentx;
  m_Tangent[tangN][1] = tangenty;
  m_Tangent[tangN][2] = tangentz;
}

/** Get the tangent at one point */
template< unsigned int TPointDimension >
const typename SparsePointsSpatialObjectPoint< TPointDimension >::TangentType &
SparsePointsSpatialObjectPoint< TPointDimension >
::GetTangent( unsigned int tangN ) const
{
  if(tangN >= m_NumberOfTangents)
    {
    ExceptionObject exception(__FILE__, __LINE__);
    exception.SetDescription("Requested a non-existent tangent direction");
    throw exception;
    }
  return m_Tangent[tangN];
}
  
/** Get the weight of the tangent at a point */
template< unsigned int TPointDimension >
void
SparsePointsSpatialObjectPoint< TPointDimension >
::SetTangentWeight( unsigned int tangN, double tWeight ) 
{
  if(tangN >= m_NumberOfTangents)
    {
    ExceptionObject exception(__FILE__, __LINE__);
    exception.SetDescription("Tried to set weight of a non-existent tangent");
    throw exception;
    }
  m_TangentWeight[tangN] = tWeight;
}
  
/** Get the weight of the tangent at a point */
template< unsigned int TPointDimension >
double
SparsePointsSpatialObjectPoint< TPointDimension >
::GetTangentWeight( unsigned int tangN ) const
{
  if(tangN >= m_NumberOfTangents)
    {
    ExceptionObject exception(__FILE__, __LINE__);
    exception.SetDescription("Tried to get weight of a non-existent tangent");
    throw exception;
    }
  return m_TangentWeight[tangN];
}

/** Print the object */
template< unsigned int TPointDimension >
void 
SparsePointsSpatialObjectPoint< TPointDimension >
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  Superclass::PrintSelf(os,indent);
  os << indent << "SparsePointsSpatialObjectPoint(" << this << ")" << std::endl; 
  os << indent << "Scale = " << m_Scale << std::endl;
  os << indent << "Weight = " << m_Weight << std::endl;
  os << indent << "Number of Normals = "
               <<  m_NumberOfNormals << std::endl;
  os << indent << "Normals: " << std::endl;
  for(int i=0; i< m_NumberOfNormals; i++)
    {
    os << indent <<  m_Normal[i] << std::endl;
    }
  os << indent << "Normal Weights: " << std::endl;
  for(int i=0; i< m_NumberOfNormals; i++)
    {
    os << indent <<  m_NormalWeight[i] << std::endl;
    }
  os << indent << "Number of Tangents = "
               <<  m_NumberOfTangents << std::endl;
  os << indent << "Tangents: " << std::endl;
  for(int i=0; i< m_NumberOfTangents; i++)
    {
    os << indent <<  m_Tangent[i] << std::endl;
    }
  os << indent << "Tangent Weights: " << std::endl;
  for(int i=0; i< m_NumberOfTangents; i++)
    {
    os << indent <<  m_TangentWeight[i] << std::endl;
    }
}

/** Copy a surface point to another */
template< unsigned int TPointDimension >
typename SparsePointsSpatialObjectPoint< TPointDimension >::Self & 
SparsePointsSpatialObjectPoint< TPointDimension >
::operator=(const SparsePointsSpatialObjectPoint & rhs) 
{
  this->m_ID = rhs.m_ID;
  this->m_X = rhs.m_X;
  this->m_Color = rhs.m_Color;
  m_Scale = rhs.m_Scale;
  m_Weight = rhs.m_Weight;
  m_NumberOfNormals = rhs.m_NumberOfNormals;
  m_Normal = rhs.m_Normal;
  m_NormalWeight = rhs.m_NormalWeight;
  m_NumberOfTangents = rhs.m_NumberOfTangents;
  m_Tangent = rhs.m_Tangent;
  m_TangentWeight = rhs.m_TangentWeight;
  m_DisplacementVector = rhs.m_DisplacementVector;
  std::cout << "m_DisplacementVector = " << m_DisplacementVector << std::endl;
  return * this;
}

} // end namespace itk

#endif
