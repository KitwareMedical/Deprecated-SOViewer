/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceTubeVTKRenderMethodSlicer.h,v $
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
#ifndef __SurfaceTubeVTKRenderMethodSlicer_h
#define __SurfaceTubeVTKRenderMethodSlicer_h

#include "sovVTKRenderMethodSlicer.h"
#include "itkTubeSpatialObject.h"
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT SurfaceTubeVTKRenderMethodSlicer
:public VTKRenderMethodSlicer
{

public: 

  typedef SurfaceTubeVTKRenderMethodSlicer        Self;
  typedef SmartPointer< Self >                Pointer;
  typedef VTKRenderMethodSlicer                   Superclass;
  
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;
  typedef Superclass::ObjectListType          ObjectListType;

  itkNewMacro( SurfaceTubeVTKRenderMethodSlicer );

  itkTypeMacro( SurfaceTubeVTKRenderMethodSlicer, VTKRenderMethodSlicer );

  virtual void draw( void );
  virtual void Update();

protected:

  SurfaceTubeVTKRenderMethodSlicer();

  ~SurfaceTubeVTKRenderMethodSlicer();

  void PrintSelf (std::ostream &os, Indent indent) const;


  
  unsigned int m_NumberOfSides;

};

} // end of namespace sov

#endif //__SurfaceTubeVTKRenderMethodSlicer_h
