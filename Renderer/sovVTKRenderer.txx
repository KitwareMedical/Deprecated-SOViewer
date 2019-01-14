/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRenderer.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-24 16:35:10 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovVTKRenderer.h"

namespace sov
{

/** Constructor */
template < unsigned int TDimension >
VTKRenderer< TDimension >
::VTKRenderer()
{ 
  m_Renderer = vtkRenderer::New();
  m_Renderer->SetBackground(0.5,0.5,0.5);
  this->m_Type = "VTK";
}

/** Destructor */
template < unsigned int TDimension >
VTKRenderer< TDimension >
::~VTKRenderer()
{
  m_Renderer->Delete();
}

/** Set the background color */
template < unsigned int TDimension >
void
VTKRenderer< TDimension >
::SetBackground(float R,float G, float B)
{
  m_Renderer->SetBackground(R,G,B);
}


template < unsigned int TDimension >
void
VTKRenderer< TDimension >
::PrintSelf( std::ostream& itkNotUsed(os), Indent itkNotUsed(indent) ) const
{
  //Superclass::PrintSelf(os,indent);
}

template < unsigned int TDimension >
void
VTKRenderer< TDimension >
::draw()
{
  Superclass::draw();
}

} //end of namespace sov
