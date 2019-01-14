/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVisualTubeProperty.h,v $
  Language:  C++
  Date:      $Date: 2005-06-21 18:40:02 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __VisualTubeProperty_h
#define __VisualTubeProperty_h

#include "sovVisualObjectProperty.h"

namespace sov
{

class VisualTubeProperty
: public VisualObjectProperty
{

public:


  VisualTubeProperty()
  {
    m_ShowExtremities = true;
    m_EllipseFactor = 1000;
  }
  
  ~VisualTubeProperty(){};

  void SetShowExtremities(bool value)
  {
    m_ShowExtremities = value;
    this->Modified();
  }


  bool GetShowExtremities(void)
  {
    return m_ShowExtremities;
  }

 void SetEllipseFactor(float value)
  {
    m_EllipseFactor = value;
    this->Modified();
  }


  float GetEllipseFactor(void)
  {
    return m_EllipseFactor;
  }


protected:

  bool m_ShowExtremities;
  float m_EllipseFactor; // use by the DTI Tube

};

} //end of namespace sov

#endif //__sovVisualTubeProperty_h
