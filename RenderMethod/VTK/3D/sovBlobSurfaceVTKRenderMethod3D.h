/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobSurfaceVTKRenderMethod3D.h,v $
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
#ifndef __BlobSurfaceVTKRenderMethod3D_h
#define __BlobSurfaceVTKRenderMethod3D_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "sovVTKRenderMethod3D.h"
#include "itkBlobSpatialObject.h"

#include <vtkDataSetMapper.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkThinPlateSplineTransform.h>
#include <vtkTransformFilter.h>
#include <vtkFloatArray.h>
#include <vtkRectilinearGrid.h>
#include <vtkRectilinearGridGeometryFilter.h>
#include <vtkMath.h>
#include <vtkCellArray.h>
#include <vtkVRMLExporter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT BlobSurfaceVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef BlobSurfaceVTKRenderMethod3D     Self;
  typedef SmartPointer< Self >             Pointer;
  typedef VTKRenderMethod3D                Superclass;
  
  typedef itk::BlobSpatialObject<3>        BlobType;
  typedef BlobType::Pointer                BlobPointer;
  typedef BlobType::BlobPointType          BlobPointType;
  typedef BlobPointType*                   BlobPointPointer;
  typedef BlobType::PointListType          BlobPointListType;
  typedef BlobPointListType *              BlobPointListPointer;
  typedef BlobPointType::PointType         PointType;
  typedef PointType *                      PointPointer;

  itkNewMacro( BlobSurfaceVTKRenderMethod3D );

  itkTypeMacro( BlobSurfaceVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

  itkSetMacro(Smoothness,double);
  itkGetMacro(Smoothness,double);

  itkSetMacro(Decimation,double);
  itkGetMacro(Decimation,double);
    
  itkSetMacro(Scale,double);
  itkGetMacro(Scale,double);
protected:

  BlobSurfaceVTKRenderMethod3D();

  ~BlobSurfaceVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

  double m_Smoothness;
  double m_Decimation;
  double m_Scale;

};

} // end of namespace sov

#endif //__BlobSurfaceVTKRenderMethod3D_h
