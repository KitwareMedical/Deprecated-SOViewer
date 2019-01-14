/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovMouseInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MouseInteractor_h
#define __MouseInteractor_h

#include <sovPushingInteractor.h>

/** A mouse interactor returns a 2D coordinates but can work for any scene dimensions */

namespace sov
  {
    class MouseInteractor : public PushingInteractor<2,3>
    {
    public:
      typedef PushingInteractor<2,3> Superclass;

      MouseInteractor(){};
      virtual ~MouseInteractor(){};

      virtual void Push(unsigned int x,unsigned y,unsigned int button){};
      virtual void Drag(unsigned int x,unsigned y,unsigned int button){};
      virtual void Release(unsigned int x,unsigned y,unsigned int button){};
      virtual void Handle(unsigned int x,unsigned int y){};
  

    };//end of class

  } //end of namespace sov

#endif //__MouseInteractor_h
