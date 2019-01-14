/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPhantomInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __sovPhantomInteractor_h
#define __sovPhantomInteractor_h

#include <iostream>
#include "sovPollingInteractor.h"
#include <GhostWrap.h>
#include <itkEuler3DTransform.h>

/** This interactor works only for a 3D scene and a return 6 parameters. 
  * The first 3 are the position and the last 3 are the euler angles */

namespace sov
  {
class PhantomInteractor : public PollingInteractor<3,6>
  {
public:
 
  typedef PollingInteractor<3,6> Superclass;

  PhantomInteractor()
    {
    m_Phantom = NULL;
    m_RootSep = NULL;
    m_Verbose = true;
    }

  virtual ~PhantomInteractor(){}

  void Update()
    {    
    gwpTransformMatrix transformMatrix = m_Phantom->getTransformMatrix();
    
    for(unsigned int i=0;i<3;i++)
      {
      for(unsigned int j=0;j<3;j++)
        {
        m_Matrix[i][j] = transformMatrix[j][i];
        }
      m_Offset[i]=transformMatrix[3][i];
      }
      
    m_Matrix = m_Matrix;
    
    UpdateObjectPosition();
    };
  
  void Start()
    {  
    // create the root separator and set it as the root of the scene graph
    m_RootSep = new gwpSeparator();
    m_GhostScene.setRoot(m_RootSep);

    // prompt the user to place the PHANToM in the reset position
    //std::cout << "Place the PHANToM in its reset position and press <ENTER>." << std::endl;
    //std::cin.get();

    // create a PHANToM instance and check to make sure it is valid
    m_Phantom = new gwpPHANToM("Default PHANToM");
    if (!m_Phantom || !m_Phantom->getValidConstruction()) 
      {
      std::cerr << "Unable to initialize PHANToM device." << std::endl;
      return;
      }
    // add the PHANToM object to the scene
    m_RootSep->addChild(m_Phantom);
    /*
    gwpConstraintEffect::Pointer constraint = new gwpConstraintEffect;
    gwpPoint pt1;
    pt1[0] = -200;
    pt1[1] = 0;
    pt1[2] = -200;
    gwpPoint pt2;
    pt2[0] = 200;
    pt2[1] = 0;
    pt2[2] = -200;
    gwpPoint pt3;
    pt3[0] = -200;
    pt3[1] = 0;
    pt3[2] = 200;
    
    constraint->gwpSetPlane(pt1,pt2,pt3);
    //constraint->setDamping(2);
    std::cout << "Damping = " << constraint->getDamping() << std::endl;
    std::cout << "Spring Stiffness = " << constraint->getSpringStiffness() << std::endl;
    m_Phantom->setEffect(constraint);
    m_Phantom->startEffect();
    */          
    // start the haptic simulation
    m_GhostScene.startServoLoop();
    if(m_Verbose)
      {
      std::cout << "Phantom Started." << std::endl;
      }
    };
  void Stop()
    {
    m_GhostScene.stopServoLoop();
    if(m_Verbose)
      {
      std::cout << "Phantom Stopped." << std::endl;
      }
    };
    
   bool GetStylusSwitch()
     {
     return m_Phantom->getStylusSwitch();
     }
     
   gwpSeparator *  GetRootSeparator() {return m_RootSep;}
    
protected:
  
  gwpScene          m_GhostScene; 
  gwpPHANToM *      m_Phantom;
  gwpSeparator *    m_RootSep;
  bool              m_Verbose;
};//end of class


  
}; //namespace

#endif //__PhantomInteractor_h
