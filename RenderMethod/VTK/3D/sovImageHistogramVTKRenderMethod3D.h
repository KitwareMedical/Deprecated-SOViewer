/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageHistogramVTKRenderMethod3D.h,v $
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
#ifndef __ImageHistogramVTKRenderMethod3D_h
#define __ImageHistogramVTKRenderMethod3D_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "sovVTKRenderMethod3D.h"

#include <itkImage.h>
#include <itkHistogram.h>
#include <itkListSample.h>
#include <itkListSampleToHistogramGenerator.h>
#include <itkVTKImageExport.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkImageSliceConstIteratorWithIndex.h>
#include <itkVector.h>

#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkTexture.h>
#include <vtkPlaneSource.h>
#include <vtkSphereSource.h>
#include <vtkImageLuminance.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkWarpScalar.h>
#include <vtkMergeFilter.h>
#include <vtkActor.h>
#include <vtkImageImport.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>
#include <vtkStructuredGrid.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkStructuredGridOutlineFilter.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkLookupTable.h>

#include <vtkCubeSource.h>
#include <vtkGlyph3D.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
#include <vtkPolyData.h>
#include <vtkTubeFilter.h>

namespace sov
{

class Property
  {
  public:

  Property()
    {
    m_Grid = vtkStructuredGrid::New();

    m_OutlineMapper = vtkPolyDataMapper::New();

    m_OutlineActor = vtkActor::New();
    m_OutlineActor->SetMapper(m_OutlineMapper);
    m_OutlineActor->GetProperty()->SetColor(1,0,0);
    m_OutlineActor->GetProperty()->SetRepresentationToWireframe();

    m_HistogramPolyData = vtkPolyData::New();

    m_HistogramLookupTable = vtkLookupTable::New();
    m_HistogramLookupTable->SetTableRange(0.0,1.0);
    m_HistogramLookupTable->SetHueRange(0.4,0.0);
    m_HistogramLookupTable->SetRampToSCurve();
    m_HistogramLookupTable->Build();

    m_HistogramMapper = vtkPolyDataMapper::New();
    m_HistogramMapper->SetLookupTable(m_HistogramLookupTable);

    m_HistogramActor = vtkActor::New();
    m_HistogramActor->SetMapper(m_HistogramMapper);
    m_HistogramActor->GetProperty()->SetInterpolationToGouraud();
    m_HistogramActor->SetScale(1,20,1);
    
    m_CutterMapper = vtkPolyDataMapper::New();
    m_CutterMapper->ScalarVisibilityOff();

    m_CutterActor = vtkActor::New();
    m_CutterActor->SetMapper(m_CutterMapper);
    m_CutterActor->GetProperty()->SetColor(1,0,0);
    };

  void Update()
    {
    vtkCubeSource * cube = vtkCubeSource::New();
    cube->SetBounds(m_HistogramActor->GetBounds());
    m_OutlineMapper->SetInput(cube->GetOutput());
    }

  void Clear()
    {
    m_OutlineMapper->Delete();
    m_OutlineActor->Delete();
    
    m_HistogramPolyData->Delete();
    m_HistogramMapper->Delete();
    m_HistogramActor->Delete();
    m_HistogramLookupTable->Delete();

    m_CutterMapper->Delete();
    m_CutterActor->Delete();

    m_Grid->Delete();
    }

  ~Property()
    {
    Clear();
    };

  unsigned long MTime;
  unsigned int  ID;

  VTKRenderMethod3D::ObjectType * m_Object;

  vtkStructuredGrid * m_Grid;

  vtkActor *          m_OutlineActor;
  vtkPolyDataMapper * m_OutlineMapper;
  
  vtkLookupTable *    m_HistogramLookupTable;
  vtkPolyData *       m_HistogramPolyData;
  vtkPolyDataMapper * m_HistogramMapper;
  vtkActor *          m_HistogramActor;

  vtkPolyDataMapper * m_CutterMapper;
  vtkActor *          m_CutterActor;
  };

template <class TImage>
class ImageHistogramVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef ImageHistogramVTKRenderMethod3D                 Self;
  typedef SmartPointer< Self >                            Pointer;
  typedef VTKRenderMethod3D                               Superclass;
 
  typedef TImage                                          ImageType;
  typedef typename ImageType::Pointer                     ImagePointer; 
  typedef typename ImageType::ConstPointer                ImageConstPointer; 
  typedef Superclass::ObjectType                          ObjectType;
  typedef Superclass::ObjectListType                      ObjectListType;
  typedef typename ImageType::PixelType                   PixelType;

  typedef ImageSpatialObject<3,PixelType>                 ImageSpatialObjectType;

  typedef itk::Image<unsigned char,2>                     InternalImageType;
  typedef InternalImageType::Pointer                      InternalImagePointer;

  typedef itk::VTKImageExport<
                   InternalImageType >                    ExportType;
  typedef vtkImageImport                                  ImportType;
  
  typedef typename itk::Vector<
          typename ImageType::PixelType,1>                VectorType;
  typedef typename itk::Statistics::ListSample<
                   VectorType >                           ListSampleType;
  typedef typename ListSampleType::Pointer                ListSamplePointer;
  typedef itk::Statistics::ListSampleToHistogramGenerator<
                   ListSampleType,
          typename ImageType::PixelType >                 HistogramGeneratorType;
  typedef typename HistogramGeneratorType::Pointer        HistogramGeneratorPointer;

  typedef typename HistogramGeneratorType::HistogramType  HistogramType;
  typedef typename HistogramType::Pointer                 HistogramPointer;
  typedef typename HistogramType::ConstPointer            HistogramConstPointer;

  itkNewMacro( ImageHistogramVTKRenderMethod3D );

  itkTypeMacro( ImageHistogramVTKRenderMethod3D, VTKRenderMethod3D );

protected:

  ImageHistogramVTKRenderMethod3D();

  ~ImageHistogramVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

  vtkCutter *     m_Cutter;
  vtkPlane *      m_CutterPlane;
  vtkTubeFilter * m_CutterWrap;
  vtkStructuredGridGeometryFilter * m_GridGeometryFilter;
  vtkSmoothPolyDataFilter * m_GridSmoothingFilter;

  std::list< Property * > m_PropertyList;
};

} // end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageHistogramVTKRenderMethod3D.txx"
#endif

#endif //__ImageHistogramVTKRenderMethod3D_h
