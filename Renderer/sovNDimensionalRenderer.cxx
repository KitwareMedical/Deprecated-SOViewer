/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovNDimensionalRenderer.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:58 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovNDimensionalRenderer.h"

namespace sov
{

/** Constructor */
NDimensionalRenderer
::NDimensionalRenderer()
{   
  m_Width = 0;
  m_Height = 0;
}

/** Destructor */
NDimensionalRenderer
::~NDimensionalRenderer()
{

}


void NDimensionalRenderer
::draw( void )
{
  std::cout << "sovNDimensionalRenderer::Draw() : Not Implemented on purpose !" << std::endl;
}



} //end of namespace sov
