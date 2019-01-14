/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovDTITubeTensorVTKRenderMethod3D.h,v $
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
#ifndef __DTITubeTensorVTKRenderMethod3D_h
#define __DTITubeTensorVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include "itkDTITubeSpatialObject.h"
#include <vtkLookupTable.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT DTITubeTensorVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef DTITubeTensorVTKRenderMethod3D   Self;
  typedef SmartPointer< Self >                Pointer;
  typedef VTKRenderMethod3D                   Superclass;
  
  typedef itk::DTITubeSpatialObject<3>        TubeType;
  typedef TubeType::Pointer                   TubePointer;
  typedef TubeType::ChildrenListType          TubeListType;
  typedef TubeType::TubePointType             TubePointType;
  typedef TubePointType*                      TubePointPointer;
  typedef TubeType::PointListType             TubePointListType;
  typedef TubePointListType *                 TubePointListPointer;
  typedef TubePointType::PointType            PointType;
  typedef PointType *                         PointPointer;

  itkNewMacro( DTITubeTensorVTKRenderMethod3D );

  itkTypeMacro( DTITubeTensorVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

protected:

  DTITubeTensorVTKRenderMethod3D();

  ~DTITubeTensorVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();
  
  unsigned int m_NumberOfSides;
};

} // end of namespace sov

#endif //__DTITubeTensorVTKRenderMethod3D_h
