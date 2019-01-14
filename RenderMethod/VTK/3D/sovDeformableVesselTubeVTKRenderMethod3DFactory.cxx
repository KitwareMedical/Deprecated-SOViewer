/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovDeformableVesselTubeVTKRenderMethod3DFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2005-03-13 22:25:38 $
  Version:   $Revision: 1.1 $
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

#include "sovDeformableVesselTubeVTKRenderMethod3D.h"
#include "sovDeformableVesselTubeVTKRenderMethod3DFactory.h"

namespace sov
{
  void 
  DeformableVesselTubeVTKRenderMethod3DFactory
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

  DeformableVesselTubeVTKRenderMethod3DFactory
  ::DeformableVesselTubeVTKRenderMethod3DFactory()
    {
    this->RegisterOverride("DeformableVesselTubeSpatialObject",
                           "DeformableVesselTubeVTKRenderMethod3D",
                           "TubeSpatialObject DeformableVessel Render Method",
                           1,
                           CreateObjectFunction<DeformableVesselTubeVTKRenderMethod3D>::New());
    }

  DeformableVesselTubeVTKRenderMethod3DFactory
  ::~DeformableVesselTubeVTKRenderMethod3DFactory()
  {
  }

  const char *
  DeformableVesselTubeVTKRenderMethod3DFactory
  ::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }

  const char *
  DeformableVesselTubeVTKRenderMethod3DFactory
  ::GetDescription() const
  {
    return "Create DeformableVesselTubeVTKRenderMethod3D for the visualization of TubeSpatialObject classes";
  }

}
