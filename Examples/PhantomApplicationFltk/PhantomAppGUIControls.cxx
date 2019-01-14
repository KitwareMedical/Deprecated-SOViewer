/*******************************************************************
 * FILE:     PhantomAppGUIControls.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/
#include "PhantomAppGUIControls.h"
#include <iostream>
#include <sovVisualImageProperty.h>
#include <list>
#include <itkTubeSpatialObject.h>
#include <itkSpatialObjectReader.h>

/** Constructor */
PhantomAppGUIControls::PhantomAppGUIControls( )
    : PhantomAppGUI(  )
{
}

/** Destructor */
PhantomAppGUIControls::~PhantomAppGUIControls()
{
  //m_PhantomRenderer->Stop();
}

void PhantomAppGUIControls::CreateScene(void)
{
  /** Create a sphere */
  /*m_Sphere = EllipseType::New();
  m_Sphere->SetRadius(1);
  m_Sphere->GetProperty()->SetColor(1,0,0);
  */ 
 /** Create a second sphere */
  m_Sphere2 = EllipseType::New();
  m_Sphere2->SetRadius(10);
  m_Sphere2->GetProperty()->SetColor(1,0,0);
  m_Sphere2->GetObjectToParentTransform()->SetOffset(10);
  
  /*typedef itk::TubeSpatialObject<3> TubeSpatialObjectType;
  TubeSpatialObjectType::Pointer tube = TubeSpatialObjectType::New();
  TubeSpatialObjectType::TubePointType pt1;
  pt1.SetPosition(0,0,0);
  pt1.SetRadius(20);
  tube->GetPoints().push_back(pt1);
  pt1.SetPosition(0,0,100);
  pt1.SetRadius(20);
  tube->GetPoints().push_back(pt1);*/
  //m_Scene->AddSpatialObject(tube);
  //m_PhantomScene->AddSpatialObject(tube);
  
  typedef itk::SpatialObjectReader<3> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName("c:/USProbeTrackerData/Skin.tre");
  reader->Update();

  itk::GroupSpatialObject<3>::Pointer group = reader->GetGroup();  
  group->ComputeBoundingBox();
  std::cout << group->GetBoundingBox()->GetBounds() << std::endl;
  
  itk::Vector<double,3> offset;
  offset[0] = -group->GetBoundingBox()->GetBounds()[0]-100;
  offset[1] = -group->GetBoundingBox()->GetBounds()[2]-100;
  offset[2] = -group->GetBoundingBox()->GetBounds()[4]-100; 
  group->GetObjectToParentTransform()->SetOffset(offset);
  /*itk::GroupSpatialObject<3>::ChildrenListType* list = group->GetChildren(9999);
  itk::GroupSpatialObject<3>::ChildrenListType::iterator it = list->begin();
  itk::TubeSpatialObject<3>::Pointer tube = static_cast<itk::TubeSpatialObject<3>*>((*it).GetPointer());
  */
 
  /** Create a small sphere to represent the stylus */
  m_Stylus = EllipseType::New();
  m_Stylus->SetRadius(2);
  m_Stylus->GetProperty()->SetColor(0,0,1);
  
  /** Define the scene */
  m_Scene = SceneType::New();
  m_Scene->AddSpatialObject(group);
  //m_Scene->AddSpatialObject(tube);
  m_Scene->AddSpatialObject(m_Sphere2);
  m_Scene->AddSpatialObject(m_Stylus);
  
  /** Create a 3D VTK Renderer */
  typedef sov::VTKRenderer3D RendererType;
  RendererType::Pointer myRenderer3D = RendererType::New();
  myRenderer3D->SetScene(m_Scene);
  Display->AddRenderer(myRenderer3D);
  Display->Update(); 
  
  /** Create the Phantom renderer */
  m_PhantomRenderer = sov::PhantomRenderer::New();
  m_PhantomRenderer->Start();
  m_PhantomRenderer->SetScene(m_Scene);
  m_PhantomRenderer->Update();
  m_PhantomRenderer->GetPhantomInteractor()->AttachWithObject(m_Stylus);
}

void PhantomAppGUIControls::Loop()
{
  while(1)
    {
    m_PhantomRenderer->GetPhantomInteractor()->Update();
    Display->Update();
    Fl::check();
    }
}

