/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovCenterLineTubeVTKRenderMethod3D.h,v $
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
#ifndef __CenterLineTubeVTKRenderMethod3D_h
#define __CenterLineTubeVTKRenderMethod3D_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif


#include "sovVTKRenderMethod3D.h"
#include "itkTubeSpatialObject.h"
#include "itkSpatialObject.h"
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT CenterLineTubeVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef CenterLineTubeVTKRenderMethod3D      Self;
  typedef SmartPointer< Self >                 Pointer;
  typedef VTKRenderMethod3D                    Superclass;
  
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::ChildrenListType          TubeListType;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;

  itkNewMacro( CenterLineTubeVTKRenderMethod3D );

  itkTypeMacro( CenterLineTubeVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  CenterLineTubeVTKRenderMethod3D();

  ~CenterLineTubeVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

};

} // end of namespace sov

#endif //__CenterLineTubeVTKRenderMethod3D_h
