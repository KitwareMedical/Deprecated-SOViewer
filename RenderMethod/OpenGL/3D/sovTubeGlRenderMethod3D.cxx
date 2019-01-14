/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovTubeGlRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-03-14 21:44:51 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovTubeGlRenderMethod3D.h"

#include "sovOpenGL.h"

namespace sov
{

  TubeGlRenderMethod3D
  ::TubeGlRenderMethod3D( void )
  {
    m_SupportedTypeList->clear();

    AddSupportedType("TubeSpatialObject");
  }

  TubeGlRenderMethod3D
  ::~TubeGlRenderMethod3D( void )
  {
  }

  void
  TubeGlRenderMethod3D
  ::draw( void )
  {
    std::cout << " TubeGlRenderMethod3D : draw method not implement yet !" << std::endl;
  }

  void
  TubeGlRenderMethod3D
  ::PrintSelf (std::ostream & itkNotUsed(os), Indent  itkNotUsed(indent)) const
  {

  }

  void
  TubeGlRenderMethod3D
  ::Update()
  {

  }

} // end of namespace sov
