/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPollingInteractor.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __PollingInteractor_h
#define __PollingInteractor_h

#include <sovInteractor.h>

namespace sov
  {
template<unsigned int TSpaceDimension,unsigned int TDOF, typename TScalarType=double>
class PollingInteractor : public Interactor<TSpaceDimension,TDOF,TScalarType>
  {
  public:
      typedef Interactor<TSpaceDimension,TDOF,TScalarType> Superclass;

      PollingInteractor(){};
      virtual ~PollingInteractor(){};

      /** This function update the interactor */
      virtual void Update() = 0;

    };//end of class

  } //end of namespace sov

#endif //__PushingInteractor_h
