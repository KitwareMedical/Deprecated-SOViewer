/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderMethod2D.cxx,v $
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
#include "sovGlRenderMethod2D.h"

namespace sov
{

GlRenderMethod2D
::GlRenderMethod2D()
{
  m_Type = "OpenGL";
  m_SupportedTypeList->clear();
  m_SliceSizeX = 0;
  m_SliceSizeY = 0;
  m_SliceNumber = 0;
  m_Orientation = 0;
  m_DisplaySlice = NULL;
  m_DisplayVolume = NULL;
}

GlRenderMethod2D
::~GlRenderMethod2D()
{

}

void
GlRenderMethod2D
::PrintSelf (std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

void
GlRenderMethod2D
::draw()
{
 
}

void 
GlRenderMethod2D
::Update()
{

}


bool 
GlRenderMethod2D::IsSupported( const char* object ) const
{
  SupportedObjectTypeListType::const_iterator it = m_SupportedTypeList->begin();
  while(it != m_SupportedTypeList->end())
  {
    if(!strcmp(object,*it))
    {
      return true;
    }
    it++;
  }

  return false;
}


} // end of namespace sov
