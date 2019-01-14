/*******************************************************************
 * FILE:     SampleAppGUIControls.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/
#include "SampleAppGUIControls.h"
#include <iostream>

#include "itkImageFileReader.h"

#include <sovVisualImageProperty.h>

#include <sovSurfaceSplineVTKRenderMethod3D.h>
#include <sovSurfaceGlRenderMethodSlicer.h>
#include <sovTubeGlRenderMethodSlicer.h>
#include <sovSurfaceTubeGlRenderMethodSlicer.h>
#include <itkSurfaceSpatialObject.h>
#include <sovGlSlicerKeyboardInteractor.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <list>

/** Constructor */
SampleAppGUIControls::SampleAppGUIControls( )
    : SampleAppGUI(  )
{
}

/**  Destroys the object and frees any allocated resources */
SampleAppGUIControls::~SampleAppGUIControls()
{
}

void SampleAppGUIControls::DisplayPosition(int x,int y ,int z,float value)
{
}

void SampleAppGUIControls::CreateScene(void)
{
  /** Create a Tube Net composed of 3 tubes */
  TubeType::PointListType list;

  float j=0;
  for( int i=-10; i<10; i++)
  {
    TubePointType p;
    p.SetPosition(i,i,i);
    p.SetRadius(1+0.1*fabs((float)i));
    p.SetRed(1);
    p.SetGreen(j);
    p.SetBlue(0);
    j+=0.05;
    list.push_back(p);
  }

  m_Tube = TubeType::New();
  m_Tube->GetProperty()->SetName("Tube 1");
  m_Tube->GetProperty()->SetRed(1);
  m_Tube->SetId(1);
  m_Tube->SetPoints(list);

  /** Define the scene */
  m_Scene = SceneType::New();
  m_Scene->AddSpatialObject(m_Tube);

  /** Create and Image */
  typedef itk::Image<PixelType, 3> myImage;
  typedef itk::ImageFileReader<myImage> ReaderType;
/*  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName("e:/Workspace/Data/BrainWeb/brainweb1e1a10f20.mha");
  reader->Update();
  myImage::Pointer m_Image = reader->GetOutput();
  */
  
  ImageType::SizeType size = {{50,50,50}};
  ImageType::IndexType index = {{0,0,0}};
  ImageType::RegionType region;
  region.SetSize( size );
  region.SetIndex( index );

  ImageType::Pointer m_Image = ImageType::New();
  m_Image->SetRegions( region );
  m_Image->Allocate();
  ImageType::SpacingType spacing;
  spacing.Fill(1);
  m_Image->SetSpacing(spacing);
 
  typedef itk::ImageRegionIteratorWithIndex<ImageType> myIteratorType;

  myIteratorType it(m_Image,region);

  it.GoToBegin();
  while( !it.IsAtEnd() ) 
    { 
    it.Set( 2438 );
    ++it;
    it.Set( 0 );
    ++it;
    }

  m_ImageSpatialObject = ImageSpatialObjectType::New();
  m_ImageSpatialObject->SetImage(m_Image);
  m_ImageSpatialObject->GetProperty()->SetName("myImage");
  m_Scene->AddSpatialObject(m_ImageSpatialObject);


  /** Create a Surface Spatial Object */
  typedef itk::SurfaceSpatialObject<3>      SurfaceType;
  typedef SurfaceType::Pointer              SurfacePointer;
  typedef itk::SurfaceSpatialObjectPoint<3> SurfacePointType;
  typedef itk::CovariantVector<double,3>    VectorType;

  SurfaceType::PointListType list1;

  for( float i=0; i<10; i+=1)
  {
    for( float j=0; j<10; j+=1)
    {
      SurfacePointType p;
      p.SetPosition(i,j,cos(i));
      VectorType normal;
      /*for(unsigned int j=0;j<3;j++)
      {
        normal[j]=j;
      }
      p.SetNormal(normal);*/
      list1.push_back(p);
    }
  }

  // Create a Surface Spatial Object
  SurfaceType::Pointer m_Surface = SurfaceType::New();
  m_Surface->GetProperty()->SetName("Surface 1");
  m_Surface->SetId(1);
  m_Surface->SetPoints(list1);

  m_Surface->ComputeBoundingBox();

  //m_Scene->AddSpatialObject(m_Surface);

  /** Create a 3D VTK Renderer */
  typedef sov::VTKRenderer3D RendererType;
  RendererType::Pointer myRenderer3D = RendererType::New();

  /** Create the Render Method and plug it to the renderer */
/*  typedef sov::ColorSurfaceTubeVTKRenderMethod3D RenderMethodType1;
  RenderMethodType1::Pointer myRenderMethod1 = RenderMethodType1::New();

  // Image Renderer Method 3D
  typedef sov::ImageVTKRenderMethod3D<ImageType> ImageRenderMethodType;
  ImageRenderMethodType::Pointer m_ImageRenderMethod3D = ImageRenderMethodType::New();

  // Bounding Box Render Method 3D 
  typedef sov::BoundingBoxVTKRenderMethod3D BBRenderMethodType;
  BBRenderMethodType::Pointer myBBRenderMethod3D = BBRenderMethodType::New();

  // Surface Render Method 3D 
  typedef sov::SurfaceSplineVTKRenderMethod3D SurfaceRenderMethodType;
  SurfaceRenderMethodType::Pointer surfaceRenderMethod3D = SurfaceRenderMethodType::New();*/
  
  // Add the object to the renderer
  //myRenderer3D->AddRenderMethod(myRenderMethod1);
  //myRenderer3D->AddRenderMethod(m_ImageRenderMethod3D);
 // myRenderer3D->AddRenderMethod(surfaceRenderMethod3D);
  
  myRenderer3D->SetScene(m_Scene);
  Display4->AddRenderer(myRenderer3D);
  Display4->Update();

  myRenderer3D->GetVisualObject(m_ImageSpatialObject)->GetProperty()->SetOpacity(0.5);
  //myRenderer3D->AssociateWithRenderMethod(m_Tube,"ColorSurfaceTubeVTKRenderMethod3D"); // Associate the object with a particular render method  

  //Display4->Update();


  // Create a 3D OpenGL Renderer
  m_RendererSlicer1 = RendererSlicerType::New();
  m_RendererSlicer1->SetName("RendererSlicer1");
/*
  // Create a 2D image renderer method
  typedef sov::ImageGlRenderMethodSlicer<ImageType> RenderMethodImageSlicerType;
  RenderMethodImageSlicerType::Pointer myRenderMethodImageSlicer = RenderMethodImageSlicerType::New();

  // Create a 2D surface renderer method
  typedef sov::SurfaceGlRenderMethodSlicer SurfaceRenderMethodSlicerType;
  SurfaceRenderMethodSlicerType::Pointer myRenderMethodSurfaceSlicer = SurfaceRenderMethodSlicerType::New();
  
  // Create a 2D tube renderer method
  typedef sov::TubeGlRenderMethodSlicer TubeRenderMethodSlicerType;
  TubeRenderMethodSlicerType::Pointer myRenderMethodTubeSlicer = TubeRenderMethodSlicerType::New();

  //myRendererSlicer->AddRenderMethod(myRenderMethodSurfaceSlicer);
  m_RendererSlicer1->AddRenderMethod(myRenderMethodImageSlicer);
  m_RendererSlicer1->AddRenderMethod(myRenderMethodTubeSlicer);
 */ 
   m_RendererSlicer1->SetScene(m_Scene);
  
  //myRendererSlicer->Active(true);
  sov::GlSlicerKeyboardInteractor* interactor1 = new sov::GlSlicerKeyboardInteractor;
  Display1->SetKeyboardInteractor(interactor1);
  Display1->AddRenderer(m_RendererSlicer1);
 
  // Create a second 3D OpenGL Renderer 
  m_RendererSlicer2 = RendererSlicerType::New();
/*  
  RenderMethodImageSlicerType::Pointer myRenderMethodImageSlicer2 = RenderMethodImageSlicerType::New();
  TubeRenderMethodSlicerType::Pointer myRenderMethodTubeSlicer2 = TubeRenderMethodSlicerType::New();
  
  m_RendererSlicer2->AddRenderMethod(myRenderMethodImageSlicer2);
  m_RendererSlicer2->AddRenderMethod(myRenderMethodTubeSlicer2);
 */
  
  m_RendererSlicer2->SetScene(m_Scene);
  //m_RendererSlicer2->SetOrientation(m_ImageSpatialObject,1); // Set the orientation of the volume to be shown
  m_RendererSlicer2->SetOrientation(1);

 // static_cast<sov::VisualImageProperty*>(m_RendererSlicer2->GetVisualObject("myImage")->GetProperty())->SetOrientation(1);
 
  Display2->AddRenderer(m_RendererSlicer2);
  sov::GlSlicerKeyboardInteractor* interactor2 = new sov::GlSlicerKeyboardInteractor;
  Display2->SetKeyboardInteractor(interactor2);
 
  // Create a second 3D OpenGL Renderer 
  m_RendererSlicer3 = RendererSlicerType::New();
  /*
  RenderMethodImageSlicerType::Pointer myRenderMethodImageSlicer3 = RenderMethodImageSlicerType::New();
  TubeRenderMethodSlicerType::Pointer myRenderMethodTubeSlicer3 = TubeRenderMethodSlicerType::New();
  
  m_RendererSlicer3->AddRenderMethod(myRenderMethodImageSlicer3);
  m_RendererSlicer3->AddRenderMethod(myRenderMethodTubeSlicer3);*/
  m_RendererSlicer3->SetScene(m_Scene);

  //static_cast<sov::VisualImageProperty*>(m_RendererSlicer3->GetVisualObject("myImage")->GetProperty())->SetOrientation(2);
  //m_RendererSlicer3->SetOrientation(m_ImageSpatialObject,2); // Set the orientation of the volume to be shown
  m_RendererSlicer3->SetOrientation(2);
  Display3->AddRenderer(m_RendererSlicer3);
  sov::GlSlicerKeyboardInteractor* interactor3 = new sov::GlSlicerKeyboardInteractor;
  Display3->SetKeyboardInteractor(interactor3);

  Slider1->maximum(m_ImageSpatialObject->GetImage()->GetLargestPossibleRegion().GetSize()[0]-1);
  Slider2->maximum(m_ImageSpatialObject->GetImage()->GetLargestPossibleRegion().GetSize()[1]-1);
  Slider3->maximum(m_ImageSpatialObject->GetImage()->GetLargestPossibleRegion().GetSize()[2]-1);
/*
  m_Scene->AddSpatialObject(myTubeN1);

  Display1->Update();
  Display2->Update();
  Display3->Update();
  Display4->Update();

  m_Scene->RemoveSpatialObject(myTubeN1);
*/

  Display1->Update();
  Display2->Update();
  Display3->Update();
  Display4->Update(); 

  static_cast<sov::VisualImageProperty*>(myRenderer3D->GetVisualObject(m_ImageSpatialObject)->GetProperty())->SetIntensityRange(0,50);
  static_cast<sov::VisualImageProperty*>(myRenderer3D->GetVisualObject(m_ImageSpatialObject)->GetProperty())->SetUseIntensityRange(true);

  static_cast<sov::VisualImageProperty*>(m_RendererSlicer3->GetVisualObject(m_ImageSpatialObject)->GetProperty())->SetIntensityRange(0,50);

}


void SampleAppGUIControls::ChangeAxialSlice(int value)
{
  m_ImageSpatialObject->SetSlicePosition(0,value);
  m_RendererSlicer1->SetSliceNumber(value);
  Display1->Update();
  Display4->Update();
}

void SampleAppGUIControls::ChangeCoronalSlice(int value)
{
  m_ImageSpatialObject->SetSlicePosition(1,value);
  m_RendererSlicer2->SetSliceNumber(value);
  Display2->Update();
 Display4->Update();
}

void SampleAppGUIControls::ChangeSaggitalSlice(int value)
{
  m_ImageSpatialObject->SetSlicePosition(2,value);
  m_RendererSlicer3->SetSliceNumber(value);
  Display3->Update();
  Display4->Update();
}
