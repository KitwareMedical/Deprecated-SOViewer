/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceSplineVTKRenderMethod3D.h,v $
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
#ifndef __SurfaceSplineVTKRenderMethod3D_h
#define __SurfaceSplineVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include "itkSurfaceSpatialObject.h"
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
#include <vtkFloatArray.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT SurfaceSplineVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef SurfaceSplineVTKRenderMethod3D      Self;
  typedef SmartPointer< Self >                Pointer;
  typedef VTKRenderMethod3D                   Superclass;
  
  typedef itk::SurfaceSpatialObject<3>        SurfaceType;
  typedef SurfaceType::Pointer                SurfacePointer;
  typedef SurfaceType::SurfacePointType       SurfacePointType;
  typedef SurfacePointType*                   SurfacePointPointer;
  typedef SurfaceType::PointListType          SurfacePointListType;
  typedef SurfacePointListType *              SurfacePointListPointer;
  typedef SurfacePointType::PointType         PointType;
  typedef PointType *                         PointPointer;

  itkNewMacro( SurfaceSplineVTKRenderMethod3D );

  itkTypeMacro( SurfaceSplineVTKRenderMethod3D, VTKRenderMethod3D );

  virtual void draw( void );

  void SetDirection(unsigned int direction){m_Direction = direction;}

protected:

  SurfaceSplineVTKRenderMethod3D();

  ~SurfaceSplineVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();
  
  void SetPoints(SurfacePointListType *);
  void ComputeThinPlateSplineTransform(void);


  vtkPoints * m_targetPoints;
  vtkPoints * m_sourcePoints;

  //================================
  // thin plate 

  vtkThinPlateSplineTransform * m_thinPlate;
  vtkTransformFilter * m_thinPlateFilter;

  //================================
  
  //================================
  // rectilinear grid
  
  vtkRectilinearGrid * m_grid;
  vtkRectilinearGridGeometryFilter * m_gridGeometry;
  vtkFloatArray * m_xGridCoords;
  vtkFloatArray * m_yGridCoords;
  vtkFloatArray * m_zGridCoords;
  float m_xGridStep;
  float m_yGridStep;
  float m_zGridStep;
  int m_xGridSize;
  int m_yGridSize;
  int m_zGridSize;
//  Mode m_representationMode;
//  Interpolation m_interpolationMode;
  bool m_dataSetLoaded;
  bool m_targetVisible;
  bool m_faceVisible;

  //================================
  
  float m_xVar;
  float m_yVar;
  float m_zVar;
  float m_sigma;

  int m_nbPoints;

  vtkFloatArray * m_xTargetCoords;
  vtkFloatArray * m_yTargetCoords;
  vtkFloatArray * m_zTargetCoords;

  vtkFloatArray * m_xSourceCoords;
  vtkFloatArray * m_ySourceCoords;
  vtkFloatArray * m_zSourceCoords;
    
  vtkUnstructuredGrid * m_target;

  vtkDataSetMapper * m_targetMapper;  
  vtkDataSetMapper * m_faceMapper;

  SurfacePointListType  m_Point3DList;
 
//  typedef itk::Image<unsigned char,2> TextureMapType;
//  TextureMapType::Pointer m_TextureMap;

  double m_Minimum[3]; // bounding box of the face 
  double m_Maximum[3]; // bounding box of the face

  vtkLookupTable * m_LookupTable;
  vtkFloatArray *     m_ColorScalars;
  vtkActor*        m_targetActor;
  vtkDataSet*      m_ThinPlatePointSet;
  const double*    m_Spacing;

  unsigned int     m_Direction; // Set the direction of the projection
};

} // end of namespace sov

#endif //__SurfaceSplineVTKRenderMethod3D_h
