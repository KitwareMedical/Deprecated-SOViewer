/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovCenterLineTubeVTKRenderMethod3DFactory.cxx,v $
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
#include <itkVersion.h>

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif


#include <cstring>

#include "sovCenterLineTubeVTKRenderMethod3D.h"
#include "sovCenterLineTubeVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  CenterLineTubeVTKRenderMethod3DFactory
  ::PrintSelf(std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os << indent <<"Current autoload path: ";
    const char * path = getenv("ITK_AUTOLOAD_PATH");
    if( path )
      {
      os<<path<<std::endl;
      }
    else
      {
      os<<"NULL"<<std::endl;
      }
  }

  CenterLineTubeVTKRenderMethod3DFactory
  ::CenterLineTubeVTKRenderMethod3DFactory()
  {
    this->RegisterOverride("TubeSpatialObject",
                           "CenterLineTubeVTKRenderMethod3D",
                           "TubeSpatialObject CenterLine Render Method",
                           1,
                           CreateObjectFunction<CenterLineTubeVTKRenderMethod3D>::New());
    
    this->RegisterOverride("VesselTubeSpatialObject",
                           "CenterLineTubeVTKRenderMethod3D",
                           "TubeSpatialObject CenterLine Render Method",
                           1,
                           CreateObjectFunction<CenterLineTubeVTKRenderMethod3D>::New());

    this->RegisterOverride("DTITubeSpatialObject",
                           "CenterLineTubeVTKRenderMethod3D",
                           "TubeSpatialObject CenterLine Render Method",
                           1,
                           CreateObjectFunction<CenterLineTubeVTKRenderMethod3D>::New());

  }

  CenterLineTubeVTKRenderMethod3DFactory
  ::~CenterLineTubeVTKRenderMethod3DFactory()
  {
  }

  const char *
  CenterLineTubeVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  CenterLineTubeVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create CenterLineTubeVTKRenderMethod3D for the visualization of TubeSpatialObject classes";
  }

}
