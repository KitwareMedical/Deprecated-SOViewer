/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: FltkSpatialObjectViewer2D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "FltkSpatialObjectViewer2D.h"

/** Constructor */
FltkSpatialObjectViewer2D::FltkSpatialObjectViewer2D()
        :FltkSpatialObjectViewer2DGUI()
{
  m_Scene = SceneType::New();
  m_Renderer = RendererType::New();
  m_Renderer->SetScene(m_Scene);
  m_Renderer->Update();
  Display->AddRenderer(m_Renderer);
  Display->Update();
}

/** Destroys the object and frees any allocated resources */
FltkSpatialObjectViewer2D::~FltkSpatialObjectViewer2D()
{
}

void FltkSpatialObjectViewer2D::AddSpatialObjectToScene( SpatialObjectType * object )
{
  m_Scene->AddSpatialObject( object );
  Display->Update();
}

void FltkSpatialObjectViewer2D::Update()
{
  Display->Update();
}

void FltkSpatialObjectViewer2D::Show()
{
  window->show();
}

//Load object button callback 
void FltkSpatialObjectViewer2D::
LoadSpatialObject()
{
  const char * file = fl_file_chooser("Select Spatial Object", "*.*", "");
  std::cout << " Loading Object " << std::endl;
  if( file != NULL )
    {
    typedef itk::SpatialObjectReader<2>          ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( file );
    reader->Update();
    GroupSpatialObjectType::Pointer group = reader->GetGroup();
    group->ComputeObjectToWorldTransform(); 
    group->ComputeObjectToParentTransform();     
    AddSpatialObjectToScene( group );
    }
}

/** Load an image spatial object */
void FltkSpatialObjectViewer2D::
LoadImage()
{
  const char * file = fl_file_chooser("Select Image File", "*.mh*", "");
  std::cout << " Loading Image " << std::endl;
  if( file != NULL )
    {
    typedef itk::ImageFileReader<ImageType>        ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( file );
    reader->Update();
    ImageType::Pointer image =  reader->GetOutput();

    if( m_Image.IsNotNull() )
      {  
      m_Scene->RemoveSpatialObject( m_Image ); 
      }
    m_Image = ImageSpatialObjectType::New();
    m_Image->SetImage(image);
    AddSpatialObjectToScene( m_Image );
    Update();
    }
}
