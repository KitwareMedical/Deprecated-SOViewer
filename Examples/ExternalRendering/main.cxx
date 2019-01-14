/*******************************************************************
 * FILE:     main.cxx
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4284 )
#pragma warning ( disable : 4018 )
#endif

#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include <itkDefaultDynamicMeshTraits.h>
#include <itkMesh.h>
#include <itkMeshSpatialObject.h>
#include <itkTriangleCell.h>
#include <itkSphereMeshSource.h>
#include <itkSceneSpatialObject.h>
#include <sovVTKRenderer3D.h>
#include <vtkAssembly.h>

int main(int , char** )
{ 

  // Some typedefs
  typedef itk::DefaultDynamicMeshTraits< float , 3, 3 > MeshTrait;
  typedef itk::Mesh<float,3,MeshTrait>                  MeshType;
  typedef MeshType::CellTraits                          CellTraits;
  typedef itk::MeshSpatialObject<MeshType>              MeshSpatialObjectType;
  typedef itk::SphereMeshSource<MeshType>         SphereMeshSourceType;
  typedef itk::SceneSpatialObject<3> SceneType;

  // Create a cubical sphere
  SphereMeshSourceType::Pointer m_MeshSource = SphereMeshSourceType::New();
  m_MeshSource->SetSquareness1(0.5);
  m_MeshSource->SetSquareness2(0.5);
  m_MeshSource->SetResolutionX(40);
  m_MeshSource->SetResolutionY(40);
  m_MeshSource->Update();

  // Get the itk::Mesh from the mesh source
  MeshType::Pointer m_Mesh = m_MeshSource->GetOutput();

  // Create the mesh Spatial Object
  MeshSpatialObjectType::Pointer m_MeshSpatialObject = MeshSpatialObjectType::New();
  m_MeshSpatialObject->SetMesh(m_Mesh);
  m_MeshSpatialObject->GetProperty()->SetColor(1.0,0.0,0.0);

  // Define the scene 
  SceneType::Pointer m_Scene = SceneType::New();
  m_Scene->AddSpatialObject(m_MeshSpatialObject);

  // Create a 3D VTK Renderer
  typedef sov::VTKRenderer3D RendererType;
  RendererType::Pointer m_SovVTKRenderer = RendererType::New();
  m_SovVTKRenderer->SetScene(m_Scene);
  m_SovVTKRenderer->Update();

  // Get the actors from the SpatialObject viewer
  vtkActorCollection* actors = m_SovVTKRenderer->GetVTKRenderer()->GetActors();
  actors->InitTraversal();
  vtkActor* actor = actors->GetNextActor();
  vtkAssembly* assembly = vtkAssembly::New();
  while(actor)
    {
    assembly->AddPart(actor);
    actor = actors->GetNextActor();
    if(actor)
      {
      actor = actors->GetNextActor();
      }
    }

  // Do the rendering in an external window
  vtkRenderer *ren1= vtkRenderer::New();
  ren1->AddActor( assembly );
  ren1->SetBackground( 0.1, 0.2, 0.4 );

  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer( ren1 );
  renWin->SetSize( 300, 300 );

  int i;
  for (i = 0; i < 360; ++i)
    {
    // render the image
    renWin->Render();
    // rotate the active camera by one degree
    ren1->GetActiveCamera()->Azimuth( 1 );
    }

  // Free up any objects we created. 
  assembly->Delete();
  ren1->Delete();
  renWin->Delete();

  return 0;
}
