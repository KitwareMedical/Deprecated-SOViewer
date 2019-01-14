/*******************************************************************
 * FILE:     FltkSpatialObjectViewerSlicer.h
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

#ifndef FltkSpatialObjectViewerSlicer_h
#define FltkSpatialObjectViewerSlicer_h

#include "FltkSpatialObjectViewerSlicerGUI.h"
#include <itkSceneSpatialObject.h>
#include <sovFlVTKDisplay.h>
#include <iostream>
#include <sovVisualImageProperty.h>
#include <sovVTKRendererSlicer.h>
#include <itkImageFileReader.h>
#include <itkSpatialObjectReader.h>
#include <itkImageSpatialObject.h>
#include <itkGroupSpatialObject.h>

#include <Fl/Fl_File_Chooser.H>
#include <Fl/Fl_Color_Chooser.H>

class FltkSpatialObjectViewerSlicer: public FltkSpatialObjectViewerSlicerGUI
{ 
public:
    
  FltkSpatialObjectViewerSlicer();
  ~FltkSpatialObjectViewerSlicer(); 

  typedef itk::SceneSpatialObject<3>          SceneType;
  typedef sov::VTKRendererSlicer              RendererType;
  typedef itk::Image<short, 3>                ImageType;
  typedef itk::ImageSpatialObject<3,short>    ImageSpatialObjectType;
  typedef itk::GroupSpatialObject<3>          GroupSpatialObjectType;
  typedef itk::SpatialObject<3>               SpatialObjectType;

  void AddSpatialObjectToScene(SpatialObjectType *);
  void SetImageSlicer( ImageType::Pointer image );
  void SetRepresentationToSurface();
  void SetRepresentationToWireframe();
  RendererType* GetRenderer() {return  m_Renderer;}
  void Show();
  void Update();

  void LoadSpatialObject();
  void LoadImage();
  void QuitViewer();
  void ChangeColor();
  void ChangeRepresentation(SOV_RepresentationType rep);
  void UpdateObjectToWorldTransform();
  void RotatePlaneX(double val);

protected :
  void Slide();
  void ActivateSlider();
  SceneType::Pointer m_Scene;
  ImageSpatialObjectType::Pointer m_Image;
  unsigned int        m_SliceDimension;   //Current Dimension of image slice that is moving
  unsigned int        m_ImageDimensionSize;  //Size of current dimension
  RendererType::Pointer m_Renderer;
};

#endif
