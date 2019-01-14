/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: itkSpatialObjectMemoryTest.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-29 23:21:02 $
  Version:   $Revision: 1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkTubeSpatialObject.h"
#include "itkEllipseSpatialObject.h"
#include <itkEuler3DTransform.h>

#include "itkImageBase.h"

#include <vnl/vnl_math.h>


int itkSpatialObjectMemoryTest(int, char *[])
{

  typedef itk::EllipseSpatialObject<3> SphereType;
  typedef itk::TubeSpatialObject<3>    TubeType;

  TubeType::Pointer m_Tube1 = TubeType::New();

  // Create a sphere
  SphereType::Pointer m_Sphere = SphereType::New();

  m_Sphere->SetRadius(5);
  m_Sphere->GetProperty()->SetRed(0);
  m_Sphere->GetProperty()->SetBlue(1);

  m_Tube1->AddSpatialObject(m_Sphere);

  TubeType::TransformType::OffsetType offset;
  offset[0]=0;
  offset[1]=0;
  offset[2]=0;
  m_Tube1->GetObjectToParentTransform()->SetOffset(offset);

  m_Tube1->ComputeObjectToWorldTransform();
 
  offset[0]=1;
  offset[1]=2;
  offset[2]=3;
  m_Sphere->GetObjectToParentTransform()->SetOffset(offset);

  typedef itk::Euler3DTransform<double> TransformType;
  TransformType::Pointer transform = TransformType::New();
  double angle = 3.141596;
  transform->SetRotation(angle,0,0);
 
  m_Sphere->GetObjectToParentTransform()->SetMatrix(transform->GetRotationMatrix());
  m_Sphere->ComputeObjectToWorldTransform();

  return EXIT_SUCCESS;

}
