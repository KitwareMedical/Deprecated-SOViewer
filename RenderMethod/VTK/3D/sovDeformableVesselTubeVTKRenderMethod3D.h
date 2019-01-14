/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovDeformableVesselTubeVTKRenderMethod3D.h,v $
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
#ifndef __DeformableVesselTubeVTKRenderMethod3D_h
#define __DeformableVesselTubeVTKRenderMethod3D_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif


#include "sovVTKRenderMethod3D.h"
#include "itkDeformableVesselTubeSpatialObject.h"
#include "itkSpatialObject.h"
#include <sovVTKWin32Header.h>
#include <vtkLookupTable.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT DeformableVesselTubeVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef DeformableVesselTubeVTKRenderMethod3D      Self;
  typedef SmartPointer< Self >                 Pointer;
  typedef VTKRenderMethod3D                    Superclass;
  
  typedef itk::DeformableVesselTubeSpatialObject<3> TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::ChildrenListType          TubeListType;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;

  itkNewMacro( DeformableVesselTubeVTKRenderMethod3D );

  itkTypeMacro( DeformableVesselTubeVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  DeformableVesselTubeVTKRenderMethod3D();

  ~DeformableVesselTubeVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

  vtkLookupTable* m_LookUpTable;

  void CreateLookUpTable();

  double GetLookUpTableIndex(float r,float g,float b);


};

} // end of namespace sov

#endif //__DeformableVesselTubeVTKRenderMethod3D_h
