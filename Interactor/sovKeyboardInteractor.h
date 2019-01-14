/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovKeyboardInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __KeyboardInteractor_h
#define __KeyboardInteractor_h

#include <sovPushingInteractor.h> 

namespace sov
  {
    class KeyboardInteractor : public PushingInteractor<1,1>
    {
    public:
      KeyboardInteractor(){};
      virtual ~KeyboardInteractor(){};

      /** This is the event when a key is pressed */
      virtual int Shortcut(int key){return 0;};


    };//end of class

  } //end of namespace sov

#endif //__KeyboardInteractor_h
