/*******************************************************************
 * FILE:     MeshDeformationGUIControls.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/
#include "MeshDeformationGUIControls.h"
#include <iostream>

#include <sovVisualImageProperty.h>


/** Constructor */
MeshDeformationGUIControls::MeshDeformationGUIControls( )
    : MeshDeformationGUI(  )
{
}

/**  Destroys the object and frees any allocated resources */
MeshDeformationGUIControls::~MeshDeformationGUIControls()
{
}

/** Try to change the pixel of the image in a random manner to test
 *  the speed of the display */
void MeshDeformationGUIControls::TestRandom(float value)
{
  m_Mesh->Initialize();
  m_MeshSource->SetSquareness1(value);
  m_MeshSource->SetSquareness2(value);
  m_MeshSource->Update();

  m_Mesh = m_MeshSource->GetOutput();
  m_MeshSpatialObject->Modified();
  m_MeshSpatialObject->GetProperty()->SetColor(value,0.0,1.0);
  Display->Update();
  Fl::check();
}



void MeshDeformationGUIControls::CreateScene(void)
{
  // Create a sphere
  m_MeshSource = SphereMeshSourceType::New();
  m_MeshSource->SetSquareness1(1.0);
  m_MeshSource->SetSquareness2(1.0);
  m_MeshSource->SetResolutionX(40);
  m_MeshSource->SetResolutionY(40);
  m_MeshSource->Update();

  m_Mesh = m_MeshSource->GetOutput();

  // Create the mesh Spatial Object
  m_MeshSpatialObject = MeshSpatialObjectType::New();
  m_MeshSpatialObject->SetMesh(m_Mesh);
  m_MeshSpatialObject->GetProperty()->SetColor(0.0,0.0,1.0);

  /** Define the scene */
  m_Scene = SceneType::New();
  m_Scene->AddSpatialObject(m_MeshSpatialObject);

  /** Create a 3D VTK Renderer */
  typedef sov::VTKRenderer3D RendererType;
  RendererType::Pointer myRenderer3D = RendererType::New();
  myRenderer3D->SetScene(m_Scene);
  Display->AddRenderer(myRenderer3D);
  Display->Update(); 
}
