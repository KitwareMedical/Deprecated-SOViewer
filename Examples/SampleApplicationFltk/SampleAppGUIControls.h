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
#include <sovVTKRenderer3D.h>
#include <sovCenterLineTubeVTKRenderMethod3D.h>
#include <itkSceneSpatialObject.h>
#include <SampleAppGUI.h>

#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include <sovImageVTKRenderMethod3D.h>
#include <sovBoundingBoxVTKRenderMethod3D.h>

#include <sovGlRendererSlicer.h>
#include <sovImageGlRenderMethodSlicer.h>

class SampleAppGUIControls : public SampleAppGUI
{ 
public:
    
  typedef short PixelType;
  typedef itk::SceneSpatialObject<> SceneType;
  typedef itk::Image<PixelType, 3> ImageType;
  typedef itk::ImageSpatialObject<3,PixelType> ImageSpatialObjectType;
  typedef sov::GlRendererSlicer RendererSlicerType;
  typedef itk::TubeSpatialObjectPoint<3>      TubePointType;
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef TubeType::Pointer                   TubePointer;

  SampleAppGUIControls();
  ~SampleAppGUIControls();
 

  void DisplayPosition(int x,int y ,int z,float value);
  void ChangeAxialSlice(int value);
  void ChangeCoronalSlice(int value);
  void ChangeSaggitalSlice(int value);

  void CreateScene(void);

protected :

  SceneType::Pointer               m_Scene;
  ImageType::Pointer               m_Image;
  ImageSpatialObjectType::Pointer  m_ImageSpatialObject;
  RendererSlicerType::Pointer      m_RendererSlicer1;
  RendererSlicerType::Pointer      m_RendererSlicer2;
  RendererSlicerType::Pointer      m_RendererSlicer3;
  TubeType::Pointer                m_Tube;

};

#endif
