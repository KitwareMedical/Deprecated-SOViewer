/*******************************************************************
 * FILE:     MeshDeformationGUIControls.h
 * PROJECT:  ITKUNC
 * AUTHORS:  CADDLab
 * DATE:     October 2002
 * COMMENTS: None
 *******************************************************************/
#ifndef MeshDeformationGUIControls_h
#define MeshDeformationGUIControls_h

#include "MeshDeformationGUI.h"
#include <sovVTKRenderer3D.h>
#include <itkSceneSpatialObject.h>
#include <MeshDeformationGUI.h>
#include <itkDefaultDynamicMeshTraits.h>
#include <itkMesh.h>
#include <itkMeshSpatialObject.h>
#include <itkTriangleCell.h>
#include <itkSphereMeshSource.h>

class MeshDeformationGUIControls : public MeshDeformationGUI
{ 
public:
    
  typedef itk::DefaultDynamicMeshTraits< float , 3, 3 > MeshTrait;
  typedef itk::Mesh<float,3,MeshTrait>                  MeshType;
  typedef MeshType::CellTraits                          CellTraits;
  typedef itk::CellInterface< float, CellTraits >       CellInterfaceType;
  typedef itk::TriangleCell<CellInterfaceType>          TriangleCellType;
  typedef itk::MeshSpatialObject<MeshType>              MeshSpatialObjectType;
  typedef MeshType::PointType                           PointType;
  typedef MeshType::CellType                            CellType;
  typedef CellType::CellAutoPointer                     CellAutoPointer;
  typedef itk::SphereMeshSource<MeshType>         SphereMeshSourceType;

  typedef itk::SceneSpatialObject<> SceneType;

  MeshDeformationGUIControls();
  ~MeshDeformationGUIControls();
 
  void CreateScene(void);
  void TestRandom(float value);

protected :

  SceneType::Pointer               m_Scene;
  MeshType::Pointer                m_Mesh;
  MeshSpatialObjectType::Pointer   m_MeshSpatialObject;
  SphereMeshSourceType::Pointer    m_MeshSource;
};

#endif
