/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: SpatialObjectManagerInteractor.h,v $
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
#ifndef __SpatialObjectManagerInteractor_h
#define __SpatialObjectManagerInteractor_h

#include <iostream>
#include <itkSpatialObject.h>
  
class SpatialObjectManagerInteractor
{

public:

  SpatialObjectManagerInteractor()
    {

    };
  ~SpatialObjectManagerInteractor(){};

  virtual void Select(itk::SpatialObject<3>* so){};


protected:

};


#endif //__SpatialObjectManagerInteractor_h
