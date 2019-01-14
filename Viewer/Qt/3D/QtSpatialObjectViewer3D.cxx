/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: QtSpatialObjectViewer3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "QtSpatialObjectViewer3D.h"

/** Constructor */
QtSpatialObjectViewer3D::QtSpatialObjectViewer3D()
{
  m_Scene = 0;
  m_Renderer = RendererType::New();
  int argc =1;
  char ** argv = new char*[1];
  //argv[0][0]=1;  
  m_Application = new QApplication(argc,argv);
  m_GUI = new QtSpatialObjectViewer3DGUI( 0, 0, TRUE, 0 );
  m_Application->setMainWidget(m_GUI);
}

/** Destroys the object and frees any allocated resources */
QtSpatialObjectViewer3D::~QtSpatialObjectViewer3D()
{
  delete m_Application;
  delete m_GUI;
}


void QtSpatialObjectViewer3D::SetScene(SceneType* scene)
{
  m_Scene = scene;
  m_Renderer->SetScene(m_Scene);
  m_Renderer->Update();
  m_GUI->Display->AddRenderer(m_Renderer);
  m_GUI->Display->Update();
  m_Renderer->GetVTKRenderer()->ResetCamera();
}

void QtSpatialObjectViewer3D::SetRepresentationToSurface()
{
  //static_cast<sov::VisualTubeProperty*>(myRenderer3D->GetVisualObject("Tube1")->GetProperty())->SetRepresentationType(SOL_SURFACE);
  //Display4->Update();
}

void QtSpatialObjectViewer3D::SetRepresentationToWireframe()
{
  //static_cast<sov::VisualTubeProperty*>(myRenderer3D->GetVisualObject("Tube1")->GetProperty())->SetRepresentationType(SOL_WIREFRAME);
  //Display4->Update();
}

void QtSpatialObjectViewer3D::Show()
{
  m_GUI->exec();
}

void QtSpatialObjectViewer3D::Update()
{
  m_GUI->Display->Update();
}
