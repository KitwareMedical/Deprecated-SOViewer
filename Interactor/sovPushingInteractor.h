/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPushingInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __PushingInteractor_h
#define __PushingInteractor_h

#include <sovInteractor.h>

namespace sov
  {
    template<unsigned int TSpaceDimension,unsigned int TDOF, typename TScalarType=double>
    class PushingInteractor : public Interactor<TSpaceDimension,TDOF,TScalarType>
    {
    public:
      PushingInteractor(){};
      virtual ~PushingInteractor(){};

    };//end of class

  } //end of namespace sov

#endif //__PushingInteractor_h
