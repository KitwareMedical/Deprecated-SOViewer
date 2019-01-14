/*******************************************************************
 * FILE:     FltkSpatialObjectViewer2D.h
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/

#ifndef FltkSpatialObjectViewer2D_h
#define FltkSpatialObjectViewer2D_h

#include "FltkSpatialObjectViewer2DGUI.h"
#include <itkSceneSpatialObject.h>
#include <sovFlGlDisplay.h>
#include <iostream>
#include <itkImageFileReader.h>
#include <itkSpatialObjectReader.h>
#include <itkImageSpatialObject.h>
#include <itkGroupSpatialObject.h>
#include <sovGlRenderer2D.h>

#include <Fl/Fl_File_Chooser.H>

/** Shared lib */
#if (defined(_WIN32) || defined(WIN32)) && !defined(SOVSTATIC) 
# ifdef FltkSOViewer2D_EXPORTS
#  define FltkSOViewer2D_EXPORT __declspec(dllexport)
# else
#  define FltkSOViewer2D_EXPORT __declspec(dllimport)
# endif 
#else
/* unix needs nothing */
#define FltkSOViewer2D_EXPORT 
#endif

class FltkSOViewer2D_EXPORT FltkSpatialObjectViewer2D: public FltkSpatialObjectViewer2DGUI
{ 
public:
    
  FltkSpatialObjectViewer2D();
  ~FltkSpatialObjectViewer2D(); 

  typedef itk::SceneSpatialObject<2>        SceneType;
  typedef sov::GlRenderer2D                 RendererType;
  typedef itk::Image<unsigned char, 2>              ImageType;
  typedef itk::ImageSpatialObject<2,unsigned char>  ImageSpatialObjectType;
  typedef itk::GroupSpatialObject<2>                GroupSpatialObjectType;
  typedef itk::SpatialObject<2>                     SpatialObjectType;

  void AddSpatialObjectToScene(SpatialObjectType *);
  void SetImageSlicer( ImageType::Pointer image );
  RendererType* GetRenderer() {return  m_Renderer;}
  void Show();
  void Update();

  void LoadSpatialObject();
  void LoadImage();

protected :

  SceneType::Pointer m_Scene;

  ImageSpatialObjectType::Pointer m_Image;
  unsigned int        m_SliceDimension;   //Current Dimension of image slice that is moving
  unsigned int        m_ImageDimensionSize;  //Size of current dimension
  RendererType::Pointer m_Renderer;
};

#endif
