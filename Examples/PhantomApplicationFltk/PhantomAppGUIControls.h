/*******************************************************************
 * FILE:     PhantomAppGUIControls.h
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/
#ifndef PhantomAppGUIControls_h
#define PhantomAppGUIControls_h

#include "PhantomAppGUI.h"
#include <itkTubeSpatialObject.h>
#include <itkEllipseSpatialObject.h>
#include <sovVTKRenderer3D.h>
#include <sovCenterLineTubeVTKRenderMethod3D.h>
#include <itkSceneSpatialObject.h>
#include <PhantomAppGUI.h>

#include <sovImageVTKRenderMethod3D.h>
#include <sovBoundingBoxVTKRenderMethod3D.h>

#include <sovGlRendererSlicer.h>
#include <sovImageGlRenderMethodSlicer.h>
#include <sovPhantomRenderer.h>

class PhantomAppGUIControls : public PhantomAppGUI
{ 
public:
    
  typedef itk::SceneSpatialObject<3>          SceneType;
  typedef itk::TubeSpatialObjectPoint<3>      TubePointType;
  typedef itk::TubeSpatialObject<3>           TubeType;
  typedef itk::EllipseSpatialObject<3>        EllipseType;  

  PhantomAppGUIControls();
  ~PhantomAppGUIControls();

  void CreateScene(void);
  void Loop(void);

protected :

  SceneType::Pointer      m_Scene;
  TubeType::Pointer       m_Tube;
  EllipseType::Pointer    m_Sphere;
  EllipseType::Pointer    m_Sphere2;  
  EllipseType::Pointer    m_Stylus;
  sov::PhantomRenderer::Pointer m_PhantomRenderer;
};

#endif
