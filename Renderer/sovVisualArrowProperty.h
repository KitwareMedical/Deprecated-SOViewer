/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVisualArrowProperty.h,v $
  Language:  C++
  Date:      $Date: 2006-03-16 21:58:33 $
  Version:   $Revision: 1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __VisualArrowProperty_h
#define __VisualArrowProperty_h

#include "sovVisualObjectProperty.h"

namespace sov
{

class VisualArrowProperty
: public VisualObjectProperty
{

public:

  VisualArrowProperty()
    {
    m_Scale = true;
    }

  bool GetScale()
   {
   return m_Scale;
   }

  void SetScale(bool scale)
   {
   m_Scale = scale;
   }

  ~VisualArrowProperty(){};

protected:

  bool   m_Scale;

};

} //end of namespace sov

#endif //__sovVisualArrowProperty_h
