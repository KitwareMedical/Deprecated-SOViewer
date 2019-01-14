/*******************************************************************
 * FILE:     FltkSpatialObjectViewer3D.h
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

#ifndef FltkSpatialObjectViewer3D_h
#define FltkSpatialObjectViewer3D_h

#include "FltkSpatialObjectViewer3DGUI.h"
#include <itkSceneSpatialObject.h>
#include <sovFlVTKDisplay.h>
#include <iostream>
#include <sovVisualImageProperty.h>
#include <sovVTKRenderer3D.h>
#include <itkImageFileReader.h>
#include <itkSpatialObjectReader.h>
#include <itkImageSpatialObject.h>
#include <itkGroupSpatialObject.h>
#include <SpatialObjectManager.h>
#include "ManagerInteractor.h"

#include <Fl/Fl_File_Chooser.H>
#include <Fl/Fl_Color_Chooser.H>


/** Shared lib */
#if (defined(_WIN32) || defined(WIN32)) && !defined(SOVSTATIC) 
# ifdef FltkSOViewer3D_EXPORTS
#  define FltkSOViewer3D_EXPORT __declspec(dllexport)
# else
#  define FltkSOViewer3D_EXPORT __declspec(dllimport)
# endif 
#else
/* unix needs nothing */
#define FltkSOViewer3D_EXPORT 
#endif

class FltkSOViewer3D_EXPORT FltkSpatialObjectViewer3D : public FltkSpatialObjectViewer3DGUI
{ 
public:
    
  FltkSpatialObjectViewer3D();
  ~FltkSpatialObjectViewer3D(); 

  typedef itk::SceneSpatialObject<3>        SceneType;
  typedef sov::VTKRenderer3D                RendererType;
  typedef itk::Image<short, 3>              ImageType;
  typedef itk::ImageSpatialObject<3,short>  ImageSpatialObjectType;
  typedef itk::GroupSpatialObject<3>        GroupSpatialObjectType;
  typedef itk::SpatialObject<3>             SpatialObjectType;

  void AddSpatialObjectToScene(SpatialObjectType *);
  //void SetScene(SceneType* scene);
  void SetImageSlicer( ImageType::Pointer image );
  void SetRepresentationToSurface();
  void SetRepresentationToWireframe();
  RendererType* GetRenderer() {return  m_Renderer;}
  void Show();
  void Update();

  void LoadSpatialObject();
  void LoadImage();
  void ScreenShot();
  void Cut();
  void QuitViewer();
  void ChangeColor();
  void ChangeRenderingResolution();
  void ChangeRepresentation(SOV_RepresentationType rep);
  void UpdateObjectToWorldTransform();
  void UpdateIWindowing();
  void ShowManager();

protected :

  void Slide();
  void ActivateSlider();
  //FltkSpatialObjectViewer3DGUI* m_GUI;
  SceneType::Pointer m_Scene;

  ImageSpatialObjectType::Pointer m_Image;
  unsigned int        m_SliceDimension;   //Current Dimension of image slice that is moving
  unsigned int        m_ImageDimensionSize;  //Size of current dimension
  RendererType::Pointer m_Renderer;
  SpatialObjectManager::Pointer m_Manager;
  ManagerInteractor* m_ManagerInteractor;
};

#endif
