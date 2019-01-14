/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovFlockOfBirdsInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __sovFlockOfBirdsInteractor_h
#define __sovFlockOfBirdsInteractor_h

#include "sovPollingInteractor.h"
#include <itkEuler3DTransform.h>
#include <Flock.h>
#include <vnl/vnl_quaternion.h>

/** This interactor works only for a 3D scene and a return 6 parameters. 
  * The first 3 are the position and the last 3 are the euler angles */

namespace sov
  {
class FlockOfBirdsInteractor : public PollingInteractor<3,6>
  {
public:
 
  typedef PollingInteractor<3,6> Superclass;
  typedef vnl_quaternion<double> VnlQuaternionType;

  FlockOfBirdsInteractor(){m_Tracker = new Flock();}
  ~FlockOfBirdsInteractor(){delete m_Tracker;}

  void Update()
    {
    m_InternalData = m_Tracker->GetData();
    
    //convert inch to mm
    for(unsigned int val=0;val<3;val++)
      {
      m_InternalData[val] *= (float)I_TO_MM;
      }

    // Change the offsets so they're in the same reference frame as the Phantom
    m_Offset[0] = -m_InternalData[0];//-m_InternalData[1];
    m_Offset[1] = -m_InternalData[2];//-m_InternalData[2];
    m_Offset[2] = -m_InternalData[1];//m_InternalData[0];
    VnlQuaternionType quaternion(m_InternalData[3],-m_InternalData[5],-m_InternalData[4],m_InternalData[6]);  
    vnl_matrix_fixed<double,3,3> rotationMatrix = quaternion.rotation_matrix_transpose();
    m_Matrix[0][0] = rotationMatrix[1][1];
    m_Matrix[1][0] = rotationMatrix[2][1];
    m_Matrix[2][0] = -rotationMatrix[0][1];
    m_Matrix[0][1] = -rotationMatrix[1][0];
    m_Matrix[1][1] = -rotationMatrix[2][0];
    m_Matrix[2][1] = rotationMatrix[0][0];
    m_Matrix[0][2] = -rotationMatrix[1][2];
    m_Matrix[1][2] = -rotationMatrix[2][2];
    m_Matrix[2][2] = rotationMatrix[0][2];

    typedef itk::Euler3DTransform<double> EulerTransformType;
    EulerTransformType::Pointer eulerTransform = EulerTransformType::New();
    eulerTransform->SetComputeZYX(true);
    static const double pi = 4*atan(1.0f); 
    //eulerTransform->SetRotation(-pi/2,-pi/2,0);
    eulerTransform->SetRotation(-pi/2,-pi,0);
    m_Matrix = eulerTransform->GetRotationMatrix()*m_Matrix;
    UpdateObjectPosition();  
    };
  
  void Start()
    {
    if(!m_Tracker->OpenComPort()) 
      { 
      std::cout << "Can't open Com port" << std::endl; 
      }
    else 
      {
      std::cout << "Tracking mode ON" << std::endl;
      }
    m_Tracker->SetMode(CONTINUOUS);
    m_Tracker->SetType(POSQUATER);
    };
  void Stop(){};
 
protected:

  Flock* m_Tracker;
  float* m_InternalData;

};//end of class


/** Update the position of the Flock of birds */  
/*void FlockOfBirdsInteractor::Update()
{
  m_InternalData = m_Tracker->GetData();
    
  //convert inch to mm
  for(unsigned int val=0;val<3;val++)
    {
    m_InternalData[val] *= (float)I_TO_MM;
    }

  // Change the offsets so they're in the same reference frame as the Phantom
  m_Offset[0] = -m_InternalData[1];
  m_Offset[1] = -m_InternalData[2];
  m_Offset[2] = m_InternalData[0];
  VnlQuaternionType quaternion(m_InternalData[3],-m_InternalData[5],-m_InternalData[4],m_InternalData[6]);  
  vnl_matrix_fixed<double,3,3> rotationMatrix = quaternion.rotation_matrix_transpose();
  m_Matrix[0][0] = rotationMatrix[1][1];
  m_Matrix[1][0] = rotationMatrix[2][1];
  m_Matrix[2][0] = -rotationMatrix[0][1];
  m_Matrix[0][1] = -rotationMatrix[1][0];
  m_Matrix[1][1] = -rotationMatrix[2][0];
  m_Matrix[2][1] = rotationMatrix[0][0];
  m_Matrix[0][2] = -rotationMatrix[1][2];
  m_Matrix[1][2] = -rotationMatrix[2][2];
  m_Matrix[2][2] = rotationMatrix[0][2];

  typedef itk::Euler3DTransform<double> EulerTransformType;
  EulerTransformType::Pointer eulerTransform = EulerTransformType::New();
  eulerTransform->ComputeZYX(true);
  static const double pi = 4*atan(1.0f); 
  eulerTransform->SetRotation(-pi/2,-pi/2,0);
  m_Matrix = eulerTransform->GetRotationMatrix()*m_Matrix;
 }*/
  
}; //namespace

#endif //__FlockOfBirdsInteractor_h
