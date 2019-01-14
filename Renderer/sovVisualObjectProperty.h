/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVisualObjectProperty.h,v $
  Language:  C++
  Date:      $Date: 2005-11-03 00:58:34 $
  Version:   $Revision: 1.5 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __VisualObjectProperty_h
#define __VisualObjectProperty_h

#include <sovNDimensionalRenderMethod.h>
#include <list>

/**
 * This class contains the visual object properties
 * If the property is modified the modified time is incremented
 * so that the render methods can keep track of the modification
 * time.
 */

typedef enum
{
  SOV_SURFACE,
  SOV_WIREFRAME,
  SOV_POINTS
} SOV_RepresentationType;

namespace sov
{

class VisualObjectProperty
{

public:

  typedef double ScalarType;

  VisualObjectProperty()
    { 
    m_Opacity = 1;
    m_Specular = 0;
    m_SpecularPower = 1;
    m_TextureName=NULL;
    m_MTime = 0;
    m_AskRenderMethodToBeUpdatedTime = 0;
    m_Visible = true;
    m_RepresentationType = SOV_SURFACE;
    m_RenderingResolution = 10;
    m_BoundingBoxVisible = false;
    m_UsePointColor = false;
    }

  ~VisualObjectProperty(){};

  void SetName(char* name)
  {
    m_Name = new char[strlen(name)+1];
    strcpy(m_Name,name);
    this->Modified();
  }


  const char * GetName(void)
  {
    return m_Name;
  }

  void SetID(unsigned int id)
  {
    m_ID = id;
    this->Modified();
  }


  unsigned int GetID(void)
  {
    return m_ID;
  }


  void SetOpacity(double opacity)
  {
    m_Opacity = opacity;
    this->Modified();
  }


  double GetOpacity(void)
  {
    return m_Opacity;
  }

  void SetSpecular(double specular)
  {
    m_Specular = specular;
    this->Modified();
  }


  double GetSpecular(void)
    {
    return m_Specular;
    }

  // Between 0 and 100
  void SetSpecularPower(double specularpower)
  {
    m_SpecularPower = specularpower;
    this->Modified();
  }


  double GetSpecularPower(void)
    {
    return m_SpecularPower;
    }

  void SetTextureFilename(char* name)
    {
    m_TextureName = name;
    this->Modified();
    }

  char* GetTextureFilename(void)
    {
    return m_TextureName;
    }

  void SetMTime(unsigned long time)
  {
    m_MTime = time;
  }

  unsigned long GetMTime(void)
    {
    return m_MTime;
    }

  void AskRenderMethodToBeUpdated()
    {
    this->m_AskRenderMethodToBeUpdatedTime++;
    }

  /** These two function add another modification time to be tested
   *  if the render method should be completely updated or not */
  unsigned long GetAskRenderMethodToBeUpdatedTime(void)
    {
    return m_AskRenderMethodToBeUpdatedTime;
    }


  void Modified(void)
    {
    m_MTime++;
    }

  void SetRepresentationType(SOV_RepresentationType type)
  {
    m_RepresentationType = type;
    this->Modified();
  }

  SOV_RepresentationType GetRepresentationType(void)
  {
    return m_RepresentationType;
  }

  void SetVisibility(bool value)
  {
    m_Visible = value;
    this->Modified();
  }

  bool GetVisibility(void) const
    {
    return m_Visible;
    }

  void BoundingBoxOn(void)
  {
    m_BoundingBoxVisible = true;
  }

  void BoundingBoxOff(void)
    {
    m_BoundingBoxVisible = false;
    }

  void BoundingBox(bool n)
    {
    m_BoundingBoxVisible = n;
    }

  bool BoundingBox(void)
    {
    if( m_BoundingBoxVisible )
      {
      return true;
      }
    return false;
    }


  void SetRenderingResolution(float resolution)
  {
    m_RenderingResolution = resolution;
  }

  float GetRenderingResolution()
  {
    return m_RenderingResolution;
  }

  void SetUsePointColor(bool usePointColor)
    {
    m_UsePointColor = usePointColor;
    }

  bool GetUsePointColor()
    {
    return m_UsePointColor;
    }

protected:

  char  *                m_Name;
  char  *                m_TextureName; 
  unsigned int           m_ID;
  double                 m_Opacity;
  double                 m_Specular;
  double                 m_SpecularPower;
  SOV_RepresentationType m_RepresentationType;
  bool                   m_Visible;
  bool                   m_BoundingBoxVisible;
  float                  m_RenderingResolution;
  bool                   m_UsePointColor;

  unsigned long m_MTime;
  unsigned long m_AskRenderMethodToBeUpdatedTime; // this allows to update the rendermethod if the property is modified

};

} //end of namespace sov

#endif //__sovVisualObjectProperty_h
