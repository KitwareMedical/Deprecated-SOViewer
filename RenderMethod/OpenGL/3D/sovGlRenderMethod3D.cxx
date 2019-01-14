/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderMethod3D.cxx,v $
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
#include "sovGlRenderMethod3D.h"

namespace sov
{

GlRenderMethod3D
::GlRenderMethod3D()
{
  m_Type = "OpenGL";
  m_SupportedTypeList->clear();
}

GlRenderMethod3D
::~GlRenderMethod3D()
{

}

void
GlRenderMethod3D
::PrintSelf (std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

void
GlRenderMethod3D
::draw()
{
}

void 
GlRenderMethod3D
::Update()
{
}


bool 
GlRenderMethod3D::IsSupported( const char* object ) const
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
