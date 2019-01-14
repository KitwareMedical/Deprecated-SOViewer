/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkUltraSoundProbeSpatialObject.cxx,v $
  Language:  C++
  Date:      $Date: 2005-04-28 14:42:15 $
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

#include "itkUltrasoundProbeSpatialObject.h" 
#include "itkEllipseSpatialObject.h" 

namespace itk  
{ 

/** Constructor */
UltrasoundProbeSpatialObject
::UltrasoundProbeSpatialObject()  
{ 
  this->SetDimension(3);
  this->SetTypeName("UltrasoundProbeSpatialObject");
  this->GetProperty()->SetRed(1); 
  this->GetProperty()->SetGreen(0); 
  this->GetProperty()->SetBlue(0); 
  this->GetProperty()->SetAlpha(1); 
  ComputeBoundingBox();
  m_Sensor = NULL;
  m_Tip = NULL;
  m_Body = NULL;
  m_OverlapX = 100;
  m_BodyShiftZ = 0;
  m_BodySizeY = 40;
} 
 
/** Destructor */
UltrasoundProbeSpatialObject 
::~UltrasoundProbeSpatialObject()
{ 
}

/** Construct the probe */
void
UltrasoundProbeSpatialObject
::Construct()
{ 
  m_Sensor = RectangleType::New();
  RectangleType::SizeType rectSize;
  rectSize.Fill(20);
  m_Sensor->SetSize(rectSize);
  m_Sensor->GetProperty()->SetColor(1,0,0);
  this->AddSpatialObject(m_Sensor);

  RectangleType::Pointer ruler = RectangleType::New();
  RectangleType::SizeType rulerSize;
  rulerSize[0]=300;
  rulerSize[1]=20;
  rulerSize[2]=20;
  ruler->SetSize(rulerSize); // in mm
  this->AddSpatialObject(ruler);
  m_Sensor->AddSpatialObject(ruler);
  OffsetType offset1;
  offset1[0]=ruler->GetSize()[0]/2-m_Sensor->GetSize()[0]/2;
  offset1[1]=-ruler->GetSize()[1]/2-m_Sensor->GetSize()[1]/2;
  offset1[2]=0;
  ruler->GetNodeToParentNodeTransform()->SetOffset(offset1);
  ruler->ComputeObjectToWorldTransform();
  ruler->GetProperty()->SetColor(1,1,1);

  
  m_Body = RectangleType::New();
  ruler->AddSpatialObject(m_Body);
  RectangleType::SizeType bodySize;
  bodySize[0]=150;
  bodySize[1]=m_BodySizeY;
  bodySize[2]=60;
  m_Body->SetSize(bodySize);
  OffsetType offsetBody;
  
  offsetBody[0]=m_Body->GetSize()[0]/2+ruler->GetSize()[0]/2-m_OverlapX;
  offsetBody[1]=m_Body->GetSize()[1]/2+ruler->GetSize()[1]/2;
  offsetBody[2]=m_BodyShiftZ;
  m_Body->GetNodeToParentNodeTransform()->SetOffset(offsetBody);
  m_Body->ComputeObjectToWorldTransform();
  m_Body->GetProperty()->SetColor(1,1,1);

  m_Tip = SphereType::New();
  double scale[3];
  scale[0] = 30;
  scale[1] = m_Body->GetSize()[1]/2;
  scale[2] = m_Body->GetSize()[2]/2;
  m_Tip->SetRadius(1);
  m_Body->AddSpatialObject(m_Tip);
  OffsetType offsetSphere;
  offsetSphere[0] = m_Body->GetSize()[0]/2;
  offsetSphere[1] = 0;
  offsetSphere[2] = 0;

  m_Tip->GetNodeToParentNodeTransform()->SetOffset(offsetSphere);
  m_Tip->GetObjectToNodeTransform()->SetScaleComponent(scale);
  m_Tip->ComputeObjectToWorldTransform();
  m_Tip->GetProperty()->SetColor(1,1,1);
}



/** Print the object */ 
void  
UltrasoundProbeSpatialObject
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "UltrasoundProbeSpatialObject(" << this << ")" << std::endl; 
  Superclass::PrintSelf( os, indent );
} 
 
} // end namespace itk 

