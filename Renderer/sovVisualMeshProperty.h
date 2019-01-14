/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVisualMeshProperty.h,v $
  Language:  C++
  Date:      $Date: 2005-12-15 19:50:37 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __VisualMeshProperty_h
#define __VisualMeshProperty_h

#include "sovVisualObjectProperty.h"
#include <list>
#include "sovColorScheme.h"

namespace sov
{


struct VisualMeshPoint
{
  unsigned long Id;
  float r;
  float g;
  float b;

} ;

class VisualMeshProperty
: public VisualObjectProperty
{

public:

  typedef std::list<VisualMeshPoint> PointListType;

  VisualMeshProperty()
  {
  m_PixelType="";
  }

  const char* GetPixelType()
    {
    return m_PixelType.c_str();
    }

  void SetPixelType(std::string type)
    {
    m_PixelType = type;
    }

  const VisualMeshPoint* GetVisualPoint(unsigned long id)
    {
    PointListType::const_iterator it = m_PointList.begin();
    for(;it != m_PointList.end();it++)
      {
      if((*it).Id == id)
        {
        return &(*it);
        }
      }
    return NULL;
    }

  ColorScheme *GetColorScheme() { return &m_ColorScheme;}

  PointListType & GetPointList() {return m_PointList;}

  ~VisualMeshProperty(){};


protected:
  std::string   m_PixelType;
  PointListType m_PointList;
  ColorScheme m_ColorScheme ;
};

} //end of namespace sov

#endif //__sovVisualMeshProperty_h
