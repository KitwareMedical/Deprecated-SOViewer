/*******************************************************************
 * FILE:     SampleAppGUIControls.h
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

#ifndef SampleAppGUIControls_h
#define SampleAppGUIControls_h

#include "SampleAppGUI.h"
#include <itkImage.h>
#include <itkTubeSpatialObject.h>
#include <itkGroupSpatialObject.h>
#include <sovVTKRenderer3D.h>

#include <itkEllipseSpatialObject.h>
#include <sovCenterLineTubeVTKRenderMethod3D.h>
#include <itkSceneSpatialObject.h>
#include <SampleAppGUI.h>
#include <sovQtVTKDisplay.h>
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include <sovImageVTKRenderMethod3D.h>
#include <sovBoundingBoxVTKRenderMethod3D.h>

#include <sovGlRendererSlicer.h>
#include <sovImageGlRenderMethodSlicer.h>
#include <sovQtGlDisplay.h>

#include <iostream>
#include <qslider.h>

#include "itkImageFileReader.h"
#include "itkMetaImageIOFactory.h"

#include <sovVisualImageProperty.h>

#include <sovSurfacePointVTKRenderMethod3D.h>
#include <sovSurfaceSplineVTKRenderMethod3D.h>
#include <sovSurfaceGlRenderMethodSlicer.h>
#include <itkSurfaceSpatialObject.h>
#include <list>

class SampleAppGUIControls : public SampleAppGUI
{ 
public:
    
  typedef unsigned char PixelType;
  typedef itk::SceneSpatialObject<> SceneType;
  typedef itk::Image<PixelType, 3> ImageType;
  typedef itk::ImageSpatialObject<3,PixelType> ImageSpatialObjectType;
  typedef sov::GlRendererSlicer RendererSlicerType;
  typedef sov::VTKRenderer3D RendererType;
  typedef itk::TubeSpatialObjectPoint<3>      TubePointType;
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef itk::GroupSpatialObject<3>    TubeNetType;
  typedef TubeType::Pointer                   TubePointer;
  typedef itk::EllipseSpatialObject<3> SphereType;

  SampleAppGUIControls( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~SampleAppGUIControls();
 

  void DisplayPosition(int x,int y ,int z,float value);
  void ChangeAxialSlice(int value);
  void ChangeCoronalSlice(int value);
  void ChangeSaggitalSlice(int value);

  void CreateScene(void);
  void Scale(int scale);

  void SetRepresentationToSurface();
  void SetRepresentationToWireframe();

  void MoveTube(int value);
  void MoveSphere(int value);
  void RotateTube(int value);
  void RotateSphere(int value);

protected :

  SceneType::Pointer m_Scene;
  ImageType::Pointer  m_Image;
  RendererSlicerType::Pointer m_RendererSlicer;
  RendererSlicerType::Pointer m_RendererSlicer2;
  RendererSlicerType::Pointer m_RendererSlicer3;
  RendererType::Pointer myRenderer3D;
  ImageSpatialObjectType::Pointer m_ImageSpatialObject;
  TubeNetType::Pointer m_TubeNet1;
  SphereType::Pointer m_Sphere;
  TubePointer m_Tube1;

};

#endif
