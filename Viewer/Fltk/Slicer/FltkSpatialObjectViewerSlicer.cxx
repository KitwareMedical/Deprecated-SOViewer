/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: FltkSpatialObjectViewerSlicer.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-29 23:21:41 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "FltkSpatialObjectViewerSlicer.h"
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkTubeFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>

#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h> 
#include <vtkFloatArray.h>
#include <sovVisualTubeProperty.h>
#include <vtkProperty.h>
#include <vtkClipPolyData.h>
#include <vtkSphere.h>

/** Constructor */
FltkSpatialObjectViewerSlicer::FltkSpatialObjectViewerSlicer()
        :FltkSpatialObjectViewerSlicerGUI()
{
  m_Scene = SceneType::New();
  m_Renderer = RendererType::New();
  //slider->deactivate();
  slider->activate();
  slider->slider_size( 1/2000 ); 
  slider->value( 0, (int)0.01, 0, 1000);

  m_SliceDimension = 0;  
  gui_xy->value(1);
  m_Renderer->SetScene(m_Scene);
  m_Renderer->Update();
  Display->AddRenderer(m_Renderer);
  Display->Update();
  //Display->DisableEvents(); // disable interactions
  //
  //m_Renderer->GetVTKRenderer()->ResetCamera();
}

/** Destroys the object and frees any allocated resources */
FltkSpatialObjectViewerSlicer::~FltkSpatialObjectViewerSlicer()
{
}



//*** general quit  ***//
void FltkSpatialObjectViewerSlicer::QuitViewer()
{
    exit(0);
}

/** Update the object to world transform */
void FltkSpatialObjectViewerSlicer::UpdateObjectToWorldTransform()
{
}  


/** Change the color of the selected object */
void FltkSpatialObjectViewerSlicer::ChangeColor()
{ 
} 


/** Change the representation type of the selected object */
void FltkSpatialObjectViewerSlicer::ChangeRepresentation(SOV_RepresentationType rep) 
{
}

/** Rotate the plane */
void FltkSpatialObjectViewerSlicer::RotatePlaneX(double value)
{
/*  vtkPlaneSource* plane = m_Renderer->GetPlane();

  itk::Vector<double,3> normal;
  normal.Fill(0);
  normal[2] = 1.0;
  typedef itk::Euler3DTransform<double> TransformType;
  TransformType::Pointer transform = TransformType::New();
  transform->SetRotation(0,value,0);
  normal = transform->TransformVector(normal);  
  plane->SetNormal(normal[0],normal[1],normal[2]);*/

  m_Renderer->SetAngles(0,value,0);
  Display->Update();
}


void FltkSpatialObjectViewerSlicer::SetImageSlicer( ImageType::Pointer image )
{
  if( m_Image.IsNotNull() )
  {  m_Scene->RemoveSpatialObject( m_Image ); }

  m_Image = ImageSpatialObjectType::New();
  m_Image->SetImage(image);
  AddSpatialObjectToScene( m_Image );
  ActivateSlider();
}

void FltkSpatialObjectViewerSlicer::AddSpatialObjectToScene( SpatialObjectType * object )
{
  m_Scene->AddSpatialObject( object );
  Display->Update();
  //m_Renderer->GetVTKRenderer()->ResetCamera();
}

void FltkSpatialObjectViewerSlicer::Update()
{
  Display->Update();
}

void FltkSpatialObjectViewerSlicer::SetRepresentationToSurface()
{
}

void FltkSpatialObjectViewerSlicer::SetRepresentationToWireframe()
{
}

void FltkSpatialObjectViewerSlicer::Show()
{
  window->show();
}

void FltkSpatialObjectViewerSlicer::ActivateSlider()
{
  if( gui_xy->value() )
  { m_SliceDimension = 2; }
  else if( gui_yz->value() )
  { m_SliceDimension = 0; }
  else if( gui_xz->value() )
  { m_SliceDimension = 1;  }
  else { std::cout << " ERROR in viewer: please close and open again. " << std::endl; return;  }

  m_Renderer->SetOrientation(m_SliceDimension);
  Display->Update();

  m_ImageDimensionSize = m_Image->GetImage()->GetLargestPossibleRegion().GetSize()[m_SliceDimension];
  slider->slider_size( 1/m_ImageDimensionSize ); 
  slider->value( m_Image->GetSlicePosition( m_SliceDimension ), 1, 0, m_ImageDimensionSize );

  slider->activate();
}

void FltkSpatialObjectViewerSlicer::Slide()
{
  float value = slider->value();
  //m_Image->SetSlicePosition( m_SliceDimension, slider->value() );
  //m_Renderer->SetDepth(value);
  
  
  m_Renderer->SetOffset(0,value,0);

  
  Update();
}

//Load object button callback 
void FltkSpatialObjectViewerSlicer::
LoadSpatialObject()
  {
  const char * file = fl_file_chooser("Select Spatial Object", "*.*", "");
  //try{
    std::cout << " Loading Object " << std::endl;
    if( file != NULL )
    {
    typedef itk::SpatialObjectReader<>          ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( file );
    reader->Update();
    GroupSpatialObjectType::Pointer group = reader->GetGroup();
    
    group->ComputeObjectToWorldTransform(); 
    group->ComputeObjectToParentTransform(); 
    //oup->GetObjectToParentTransform()->Print(std::cout);
    
    AddSpatialObjectToScene( group );
    //Update();
    }
  /*  }
  catch( ... )
    {
    std::cout << "There was an error in specified filename" << std::endl;
    }*/
  }

  //Load object button callback 
void FltkSpatialObjectViewerSlicer::
LoadImage()
  {
  const char * file = fl_file_chooser("Select Image File", "*.mh*", "");
 // try{
    std::cout << " Loading Image " << std::endl;
    if( file != NULL )
    {
    typedef itk::ImageFileReader<ImageType>        ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( file );
    reader->Update();
    SetImageSlicer( reader->GetOutput() );
    Update();
    }
 /*   }
  catch( itk::ExceptionObject &e )
    {
    std::cout << "There was an error in specified image filename" << std::endl;
    std::cout << "Error: " << e.GetDescription() << std::endl;
   }
    catch( std::exception &e )
    {
      std::cout << "There was an error in specified image filename" << std::endl;
    }
    catch( ... )
    {}
*/
  }
