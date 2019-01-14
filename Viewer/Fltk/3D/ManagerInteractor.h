/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: ManagerInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __ManagerInteractor_h
#define __ManagerInteractor_h

#include <iostream>
#include <SpatialObjectManagerInteractor.h>
 
class ManagerInteractor : public  SpatialObjectManagerInteractor
{

public:

  typedef itk::SpatialObject<3> SpatialObjectType;
  ManagerInteractor()
    {
    selectedSO = NULL;
    m_GUI = NULL;
    };
  ~ManagerInteractor(){};

  void Select(SpatialObjectType* so);

  void SetGUI(void* gui) {m_GUI = gui;}

  SpatialObjectType* GetSelectedSO() {return selectedSO.GetPointer();}



protected:

  void* m_GUI;
  SpatialObjectType::Pointer selectedSO;

};


#endif //__SpatialObjectManagerInteractor_h
