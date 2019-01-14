/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: SampleAppGUIControls.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-29 23:19:17 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "SampleAppGUIControls.h"
#include "sovTubeGlRenderMethodSlicer.h"
#include "sovSurfaceTubeGlRenderMethodSlicer.h"
#include <sovVisualObjectProperty.h>
#include <sovSphereVTKRenderMethod3D.h>
#include <itkEuler3DTransform.h>
#include <itkImageRegionIteratorWithIndex.h>

/** Constructor */
SampleAppGUIControls::SampleAppGUIControls( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : SampleAppGUI( parent, name, fl )
{

}

/** Destroys the object and frees any allocated resources */
SampleAppGUIControls::~SampleAppGUIControls()
{
}

void SampleAppGUIControls::DisplayPosition(int x,int y ,int z,float value)
{
}

void SampleAppGUIControls::CreateScene(void)
{
 
  
  /** Define the scene */
  m_Scene = SceneType::New();
  //m_Scene->AddSpatialObject(m_Tube);

  /** Create and Image */
  /*typedef itk::Image<PixelType, 3> myImage;
  typedef itk::ImageFileReader<myImage> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
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
    it.Set( 255 );
    ++it;
    it.Set( 100 );
    ++it;
    }


  m_ImageSpatialObject = ImageSpatialObjectType::New();
  m_ImageSpatialObject->SetImage(m_Image);
  m_ImageSpatialObject->GetProperty()->SetName("myImage");
  m_Scene->AddSpatialObject(m_ImageSpatialObject);

  /** Create a 3D VTK Renderer */
  typedef sov::VTKRenderer3D RendererType;
  RendererType::Pointer myRenderer3D = RendererType::New();
 
  myRenderer3D->SetScene(m_Scene);
  Display4->AddRenderer(myRenderer3D);
  Display4->Update();
  myRenderer3D->GetVTKRenderer()->ResetCamera();
  myRenderer3D->GetVisualObject(m_ImageSpatialObject)->GetProperty()->SetOpacity(0.5);
 

  // Create a 3D OpenGL Renderer
  m_RendererSlicer = RendererSlicerType::New();
  m_RendererSlicer->SetName("RendererSlicer1");
   m_RendererSlicer->SetScene(m_Scene);
  
 // sov::GlSliceViewDisplayInteractor* interactor1 = new sov::GlSliceViewDisplayInteractor;
//  Display1->SetInteractor(interactor1);
  Display1->AddRenderer(m_RendererSlicer);

  // Create a second 3D OpenGL Renderer 
  m_RendererSlicer2 = RendererSlicerType::New();

  
  m_RendererSlicer2->SetScene(m_Scene);
  m_RendererSlicer2->SetOrientation(1);

 
  Display2->AddRenderer(m_RendererSlicer2);
//  sov::GlSliceViewDisplayInteractor* interactor2 = new sov::GlSliceViewDisplayInteractor;
//  Display2->SetInteractor(interactor2);
 
  m_RendererSlicer3 = RendererSlicerType::New();
  m_RendererSlicer3->SetScene(m_Scene);

  m_RendererSlicer3->SetOrientation(2);
  Display3->AddRenderer(m_RendererSlicer3);
 // sov::GlSliceViewDisplayInteractor* interactor3 = new sov::GlSliceViewDisplayInteractor;
//  Display3->SetInteractor(interactor3);

  Slider1->setMaxValue(m_ImageSpatialObject->GetImage()->GetLargestPossibleRegion().GetSize()[0]-1);
  Slider2->setMaxValue(m_ImageSpatialObject->GetImage()->GetLargestPossibleRegion().GetSize()[1]-1);
  Slider3->setMaxValue(m_ImageSpatialObject->GetImage()->GetLargestPossibleRegion().GetSize()[2]-1);


  Display1->Update();
  Display2->Update();
  Display3->Update();
  Display4->Update(); 

}


void SampleAppGUIControls::ChangeAxialSlice(int value)
{
  m_ImageSpatialObject->SetSlicePosition(0,value);
  m_RendererSlicer->SetSliceNumber(value);
 
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

void SampleAppGUIControls::SetRepresentationToSurface()
{
  static_cast<sov::VisualTubeProperty*>(myRenderer3D->GetVisualObject("Tube1")->GetProperty())->SetRepresentationType(SOV_SURFACE);
  Display4->Update();
}

void SampleAppGUIControls::SetRepresentationToWireframe()
{
  static_cast<sov::VisualTubeProperty*>(myRenderer3D->GetVisualObject("Tube1")->GetProperty())->SetRepresentationType(SOV_WIREFRAME);
  Display4->Update();
}

void SampleAppGUIControls::MoveTube(int value)
{
  TubeNetType::TransformType::OffsetType offset;
  offset[0]=value;
  offset[1]=0;
  offset[2]=0;
  m_Tube1->GetObjectToParentTransform()->SetOffset(offset);
  m_Tube1->ComputeObjectToWorldTransform();
  Display4->Update();
}

void SampleAppGUIControls::MoveSphere(int value)
{
  SphereType::TransformType::OffsetType offset;
  offset[0]=value;
  offset[1]=0;
  offset[2]=0;
  m_Sphere->GetObjectToParentTransform()->SetOffset(offset);
  m_Sphere->ComputeObjectToWorldTransform();
 Display4->Update();
}

void SampleAppGUIControls::RotateTube(int value)
{
  typedef itk::Euler3DTransform<double> TransformType;
  TransformType::Pointer transform = TransformType::New();
  transform->SetRotation(0,(double(value)/180.0)*3.141596,0);

  m_Tube1->GetObjectToParentTransform()->SetMatrix(transform->GetRotationMatrix());
  m_Tube1->ComputeObjectToWorldTransform();
  Display4->Update();

}

void SampleAppGUIControls::RotateSphere(int value)
{
  typedef itk::Euler3DTransform<double> TransformType;
  TransformType::Pointer transform = TransformType::New();
  transform->SetRotation(0,(double(value)/180.0)*3.141596,0);

  m_Sphere->GetObjectToParentTransform()->SetMatrix(transform->GetRotationMatrix());
  m_Sphere->ComputeObjectToWorldTransform();
  Display4->Update();

}


void SampleAppGUIControls::Scale(int scale)
{
  double Scale[3];
  Scale[0]=(scale+1)/50.0;
  Scale[1]=(scale+1)/50.0;
  Scale[2]=(scale+1)/50.0;
  m_Sphere->GetObjectToParentTransform()->SetScaleComponent(Scale);
  m_Sphere->ComputeObjectToWorldTransform();
  Display4->Update();
}
