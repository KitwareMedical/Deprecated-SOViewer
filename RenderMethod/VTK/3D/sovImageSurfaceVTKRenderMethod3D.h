/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovImageSurfaceVTKRenderMethod3D.h,v $
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
#ifndef __ImageSurfaceVTKRenderMethod3D_h
#define __ImageSurfaceVTKRenderMethod3D_h

#include "sovVTKRenderMethod3D.h"
#include "itkImageSpatialObject.h"

#include <itkVTKImageExport.h>

#include <vtkDataSetMapper.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkThinPlateSplineTransform.h>
#include <vtkTransformFilter.h>
#include <vtkRectilinearGrid.h>
#include <vtkRectilinearGridGeometryFilter.h>
#include <vtkMath.h>
#include <vtkCellArray.h>
#include <vtkVRMLExporter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>
#include <vtkImageImport.h>
#include <vtkContourFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkDecimatePro.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageClip.h>

namespace sov
{

/**/
class ImageSurfaceEntity
  {
  public:

    ImageSurfaceEntity()
      {
      m_Clipper = vtkImageClip::New();

      m_Blur = vtkImageGaussianSmooth::New();
      m_Blur->SetInput(m_Clipper->GetOutput());
      m_Blur->SetDimensionality(3);

      m_Contour = vtkContourFilter::New();
      //m_Contour->SetInput(m_Clipper->GetOutput());
      m_Contour->SetInput(m_Blur->GetOutput());

      m_Cleaner = vtkCleanPolyData::New();
      m_Cleaner->SetInput(m_Contour->GetOutput());
      //m_Cleaner->SetInput(m_Smoother->GetOutput());
      m_Cleaner->SetTolerance(0.01);

      m_Decimater = vtkDecimatePro::New();
      m_Decimater->SetInput(m_Cleaner->GetOutput());
      m_Decimater->SetTargetReduction(0.1);
      m_Decimater->BoundaryVertexDeletionOn();
      m_Decimater->SplittingOn();
      m_Decimater->SetSplitAngle(30.0);

      m_Smoother = vtkSmoothPolyDataFilter::New();
      m_Smoother->SetInput(m_Decimater->GetOutput());
      //m_Smoother->SetInput(m_Contour->GetOutput());
      m_Smoother->SetNumberOfIterations(100);
      m_Smoother->SetRelaxationFactor(0.05);
      //m_Smoother->FeatureEdgeSmoothingOn();
      //m_Smoother->BoundarySmoothingOn();

      m_Mapper = vtkPolyDataMapper::New();
      m_Mapper->SetInput(m_Smoother->GetOutput());
      //m_Mapper->SetInput(m_Contour->GetOutput());
      m_Mapper->ScalarVisibilityOff();

      m_Actor = vtkActor::New();
      m_Actor->SetMapper(m_Mapper);
      }

    ~ImageSurfaceEntity()
      {
      Clear();
      }

    void Clear()
      {
      m_Blur->Delete();
      m_Contour->Delete();
      m_Cleaner->Delete();
      m_Decimater->Delete();
      m_Smoother->Delete();
      m_Mapper->Delete();
      m_Actor->Delete();
      }
    
    sov::VTKRenderMethod3D::ObjectType *  m_Object;

    vtkImageClip *            m_Clipper;
    vtkImageGaussianSmooth *  m_Blur;
    vtkContourFilter *        m_Contour;
    vtkCleanPolyData *        m_Cleaner;
    vtkDecimatePro *          m_Decimater;
    vtkSmoothPolyDataFilter * m_Smoother;
    vtkPolyDataMapper *       m_Mapper;
    vtkActor *                m_Actor;
  };
/**/

template <class TImage>
class ImageSurfaceVTKRenderMethod3D
:public VTKRenderMethod3D
{

public: 

  typedef ImageSurfaceVTKRenderMethod3D     Self;
  typedef SmartPointer< Self >              Pointer;
  typedef VTKRenderMethod3D                 Superclass;
  
  typedef TImage                            ImageType;
  typedef typename ImageType::PixelType     PixelType;
  typedef typename ImageType::Pointer       ImagePointer; 
  typedef typename ImageType::ConstPointer  ImageConstPointer; 
  typedef ImageSpatialObject<3,PixelType>   ImageSpatialObjectType;

  typedef itk::VTKImageExport<ImageType>    ExportType;
  typedef vtkImageImport                    ImportType;

  itkNewMacro( ImageSurfaceVTKRenderMethod3D );
  itkTypeMacro( ImageSurfaceVTKRenderMethod3D, VTKRenderMethod3D );


  virtual void draw( void );

  itkSetMacro(Smoothness,double);
  itkGetMacro(Smoothness,double);

  itkSetMacro(Decimation,double);
  itkGetMacro(Decimation,double);
    
  itkSetMacro(Scale,double);
  itkGetMacro(Scale,double);

  itkSetMacro(Threshold,double);
  itkGetMacro(Threshold,double);

protected:

  ImageSurfaceVTKRenderMethod3D();

  ~ImageSurfaceVTKRenderMethod3D();

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

  double m_Smoothness;
  double m_Decimation;
  double m_Scale;
  double m_Threshold;

  std::list<ImageSurfaceEntity *> m_EntityList;

};

} // end of namespace sov

#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovImageSurfaceVTKRenderMethod3D.txx"
#endif


#endif //__ImageSurfaceVTKRenderMethod3D_h
