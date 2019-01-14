/*******************************************************************
 * FILE:     PhantomAppGUIControls.h
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

#ifndef PhantomAppGUIControls_h
#define PhantomAppGUIControls_h

#include <GhostWrap.h>
#include "PhantomAppGUI.h"
#include <itkImage.h>
#include <solQtVTKRenderWindow.h>
#include <itkTubeSpatialObject.h>
#include <itkTubeNetworkSpatialObject.h>
#include <solVTKRenderer3D.h>


#include <solSurfaceTubeVTKRenderMethod3D.h>


#include <solCenterLineTubeVTKRenderMethod3D.h>
#include <itkTubeNetworkReader.h>
#include <itkScene.h>
#include <PhantomAppGUI.h>
#include <solQtVTKDisplay.h>
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include <solImageVTKRenderMethod3D.h>
#include <solBoundingBoxVTKRenderMethod3D.h>

#include <solGlRendererSlicer.h>
#include <solImageGlRenderMethodSlicer.h>
#include <solQtGlDisplay.h>

#include <iostream>
#include <QSlider.h>

#include "itkImageFileReader.h"
#include "itkMetaImageIOFactory.h"

#include <solVisualImageProperty.h>

#include <solSurfacePointVTKRenderMethod3D.h>
#include <solSurfaceSplineVTKRenderMethod3D.h>
#include <solSurfaceGlRenderMethodSlicer.h>
#include <itkSurfaceSpatialObject.h>
#include <list>

#include <QTimer.h>

class PhantomAppGUIControls : public PhantomAppGUI
{ 
public:
    
  typedef short PixelType;
  typedef itk::Scene<> SceneType;
  typedef itk::Image<PixelType, 3> ImageType;
  typedef itk::ImageSpatialObject<3,PixelType> ImageSpatialObjectType;
  typedef sol::GlRendererSlicer RendererSlicerType;
  typedef itk::TubeSpatialObjectPoint<3>      TubePointType;
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef itk::TubeNetworkSpatialObject<3>    TubeNetType;
  typedef TubeType::Pointer                   TubePointer;

  PhantomAppGUIControls( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~PhantomAppGUIControls();
 

  void DisplayPosition(int x,int y ,int z,float value);
  void ChangeAxialSlice(int value);
  void ChangeCoronalSlice(int value);
  void ChangeSaggitalSlice(int value);

  void CreateScene(void);
  bool StartPhantom(void);
  bool StopPhantom(void);

private slots:
  void IdleFunction();

protected :

  SceneType::Pointer m_Scene;
  ImageType::Pointer  m_Image;
  RendererSlicerType::Pointer myRendererSlicer;
  RendererSlicerType::Pointer myRendererSlicer2;
  RendererSlicerType::Pointer myRendererSlicer3;
  ImageSpatialObjectType::Pointer m_ImageSpatialObject;
  QTimer* m_Timer;

  gwpScene m_GhostScene;  // Create a GHOST scene object.
  gwpPHANToM *m_Phantom;
  TubePointer m_Tube;
  TubeNetType::Pointer m_TubeNetwork;
  gwpSeparator *m_RootSep;

};

#endif
