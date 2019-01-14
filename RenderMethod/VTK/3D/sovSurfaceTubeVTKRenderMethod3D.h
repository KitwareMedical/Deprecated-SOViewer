/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceTubeVTKRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2005-06-27 16:23:23 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __SurfaceTubeVTKRenderMethod3D_h
#define __SurfaceTubeVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include "itkTubeSpatialObject.h"
#include <vtkLookupTable.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT SurfaceTubeVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef SurfaceTubeVTKRenderMethod3D   Self;
  typedef SmartPointer< Self >                Pointer;
  typedef VTKRenderMethod3D                   Superclass;
  
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::ChildrenListType          TubeListType;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;

  itkNewMacro( SurfaceTubeVTKRenderMethod3D );

  itkTypeMacro( SurfaceTubeVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  SurfaceTubeVTKRenderMethod3D();

  ~SurfaceTubeVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();
  
  unsigned int m_NumberOfSides;

  vtkLookupTable* m_LookUpTable;

  void CreateLookUpTable();

  double GetLookUpTableIndex(float r,float g,float b);

};

} // end of namespace sov

#endif //__SurfaceTubeVTKRenderMethod3D_h
