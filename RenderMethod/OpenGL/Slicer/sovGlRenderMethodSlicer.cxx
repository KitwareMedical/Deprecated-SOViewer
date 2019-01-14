/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderMethodSlicer.cxx,v $
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
#include "sovGlRenderMethodSlicer.h"

namespace sov
{

GlRenderMethodSlicer
::GlRenderMethodSlicer()
{
  m_Type = "OpenGL";
  m_SupportedTypeList->clear();
  m_SliceSizeX = 0;
  m_SliceSizeY = 0;
  m_SliceNumber[0] = 0;
  m_SliceNumber[1] = 0;
  m_SliceNumber[2] = 0;
  m_Orientation = 0;
  m_FlipX=false;
  m_FlipY=false;
}

GlRenderMethodSlicer
::~GlRenderMethodSlicer()
{

}

void
GlRenderMethodSlicer
::PrintSelf (std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

void
GlRenderMethodSlicer
::draw()
{
 
}

void 
GlRenderMethodSlicer
::Update()
{

}


bool 
GlRenderMethodSlicer::IsSupported( const char* object ) const
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


void 
GlRenderMethodSlicer::SetDisplayVolume(unsigned char * volume )
{
  m_DisplayVolume = volume;
}


void 
GlRenderMethodSlicer::SetImageSize(unsigned int x, unsigned int y)
{
  m_SliceSizeX = x;
  m_SliceSizeY = y;
}

void 
GlRenderMethodSlicer::SetSliceNumber(unsigned int slice, unsigned int orientation )
{
  m_SliceNumber[orientation] = slice;
  this->Modified();
}

void 
GlRenderMethodSlicer::SetOrientation(unsigned int orientation)
{
  m_Orientation = orientation;
  this->Modified();
}

} // end of namespace sov
