/*******************************************************************
 * FILE:     QtSpatialObjectViewer3D.h
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

#ifndef QtSpatialObjectViewer3D_h
#define QtSpatialObjectViewer3D_h

#include "QtSpatialObjectViewer3DGUI.h"
#include <itkSceneSpatialObject.h>
#include <sovQtVTKDisplay.h>
#include <iostream>
#include <sovVisualImageProperty.h>
#include <sovVTKRenderer3D.h>
#include <qapplication.h>

class QtSpatialObjectViewer3D
{ 
public:
    
  QtSpatialObjectViewer3D();
  ~QtSpatialObjectViewer3D(); 

  typedef itk::SceneSpatialObject<3> SceneType;
  typedef sov::VTKRenderer3D RendererType;

  void SetScene(SceneType* scene);
  void SetRepresentationToSurface();
  void SetRepresentationToWireframe();
  void Show();
  void Update();

  RendererType* GetRenderer() {return m_Renderer.GetPointer();}

protected :
  QtSpatialObjectViewer3DGUI* m_GUI;
  QApplication* m_Application;
  SceneType::Pointer m_Scene;
  RendererType::Pointer m_Renderer;
};

#endif
