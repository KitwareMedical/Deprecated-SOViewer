/*=========================================================================
  Program:   itkUNC
  Module:    $RCSfile: FltkSpatialObjectViewer3D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-27 23:53:00 $
  Version:   $Revision: 1.12 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "FltkSpatialObjectViewer3D.h"
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
#include <itkMinimumMaximumImageFilter.h>

/** Constructor */
FltkSpatialObjectViewer3D::FltkSpatialObjectViewer3D()
        :FltkSpatialObjectViewer3DGUI()
{
  m_Scene = SceneType::New();
  m_Renderer = RendererType::New();
  slider->deactivate();
  m_SliceDimension = 0;  gui_xy->value(1);
  
  m_Manager = SpatialObjectManager::New();
  // Don't show the manager at startup
  //m_Manager->Show();
  m_ManagerInteractor = new ManagerInteractor;
  m_Manager->SetInteractor(m_ManagerInteractor);
  m_ManagerInteractor->SetGUI(this);
  m_Manager->SetScene(m_Scene);
  m_Renderer->SetScene(m_Scene);
  m_Renderer->Update();
  Display->AddRenderer(m_Renderer);
  Display->Update();
  m_Renderer->GetVTKRenderer()->ResetCamera();
}

/** Destroys the object and frees any allocated resources */
FltkSpatialObjectViewer3D::~FltkSpatialObjectViewer3D()
{
  delete m_ManagerInteractor;
}

/** Show the manager */
void FltkSpatialObjectViewer3D::ShowManager()
{
  m_Manager->Show();
}


/** general quit  */
void FltkSpatialObjectViewer3D::QuitViewer()
{
  window->hide();
  //exit(0);
}


/** Save the current window as BMP */
void FltkSpatialObjectViewer3D::ScreenShot()
{
 const char * file = fl_file_chooser("Save current screenshot as...", "*.bmp", "*.bmp");
 if(!file)
   {
   return;
   }

 m_Renderer->SaveRenderAsBMP(file);
}

 

/** Cut the first object with the second one */
void FltkSpatialObjectViewer3D::Cut()
{
  if(!strcmp(m_ManagerInteractor->GetSelectedSO()->GetTypeName(),"EllipseSpatialObject"))
    {
    m_Renderer->CutObject(static_cast<EllipseSpatialObject<3>*>((m_ManagerInteractor->GetSelectedSO())));
    Display->Update();
    }
}

/** Update the object to world transform */
void FltkSpatialObjectViewer3D::UpdateObjectToWorldTransform()
{
  itk::Vector<double,3> offset;
  offset[0] = OffsetX->value();
  offset[1] = OffsetY->value();
  offset[2] = OffsetZ->value();
  m_ManagerInteractor->GetSelectedSO()->GetObjectToWorldTransform()->SetOffsetComponent(offset);

  double scale[3];
  scale[0] = ScaleX->value();
  scale[1] = ScaleY->value();
  scale[2] = ScaleZ->value();
  m_ManagerInteractor->GetSelectedSO()->GetObjectToWorldTransform()->SetScaleComponent(scale);
  m_ManagerInteractor->GetSelectedSO()->ComputeObjectToParentTransform();
  Display->Update();
}  

/** Change the specular power of an object */
void FltkSpatialObjectViewer3D::ChangeRenderingResolution()
{
  // If the group is selected we propagate the color
  if(!strcmp(m_ManagerInteractor->GetSelectedSO()->GetTypeName(),"GroupSpatialObject"))
    {
    itk::GroupSpatialObject<3>::ChildrenListType* children = m_ManagerInteractor->GetSelectedSO()->GetChildren(9999999);
    itk::GroupSpatialObject<3>::ChildrenListType::iterator it = children->begin();


    while(it!=children->end())
      {
      if(m_Renderer->GetVisualObject(*it))
        {
        m_Renderer->GetVisualObject(*it)->GetProperty()->SetRenderingResolution(renderingResolution->value());
        m_Renderer->GetVisualObject(*it)->Modified();
        }
      it++;
      }
    delete children;
    }

  else if(m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO()))
    {
    m_ManagerInteractor->GetSelectedSO()->GetProperty()->SetColor(colorChooser->r(),colorChooser->g(),colorChooser->b());
    m_ManagerInteractor->GetSelectedSO()->GetProperty()->SetAlpha(colorAlpha->value());
    if(m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO()))
      {
      m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO())->GetProperty()->SetRenderingResolution(renderingResolution->value());
      m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO())->Modified();
      }
    }
  Display->Update();

}

/** Change the color of the selected object */
void FltkSpatialObjectViewer3D::ChangeColor()
{  
  if( colorAlpha->value() > 1 )
    {
    colorAlpha->value(1);
    }

  else if( colorAlpha->value() < 0 )
    {
    colorAlpha->value(0);
    }

  colorR->value(colorChooser->r());
  colorG->value(colorChooser->g());
  colorB->value(colorChooser->b());


  // If the group is selected we propagate the color
  if(!strcmp(m_ManagerInteractor->GetSelectedSO()->GetTypeName(),"GroupSpatialObject"))
    {
    itk::GroupSpatialObject<3>::ChildrenListType* children = m_ManagerInteractor->GetSelectedSO()->GetChildren(9999999);
    itk::GroupSpatialObject<3>::ChildrenListType::iterator it = children->begin();


    while(it!=children->end())
      {
      (*it)->GetProperty()->SetColor(colorChooser->r(),colorChooser->g(),colorChooser->b());
      (*it)->GetProperty()->SetAlpha( colorAlpha->value() );
      
      if(m_Renderer->GetVisualObject(*it))
        {
        m_Renderer->GetVisualObject(*it)->GetProperty()->SetVisibility(Visible->value());
        m_Renderer->GetVisualObject(*it)->GetProperty()->SetOpacity(colorAlpha->value());
        m_Renderer->GetVisualObject(*it)->GetProperty()->SetSpecular(specularLight->value());
        m_Renderer->GetVisualObject(*it)->GetProperty()->SetSpecularPower(specularPower->value());
        }
      it++;
      }
    delete children;
    }

  else if(m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO()))
    {
    m_ManagerInteractor->GetSelectedSO()->GetProperty()->SetColor(colorChooser->r(),colorChooser->g(),colorChooser->b());
    m_ManagerInteractor->GetSelectedSO()->GetProperty()->SetAlpha(colorAlpha->value());
    if(m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO()))
      {
      m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO())->GetProperty()->SetVisibility(Visible->value());
      m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO())->GetProperty()->SetSpecular(specularLight->value());
      m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO())->GetProperty()->SetOpacity(colorAlpha->value());
      m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO())->GetProperty()->SetSpecular(specularPower->value());
      }
    }
  Display->Update();
} 



/** Change the representation type of the selected object */
void FltkSpatialObjectViewer3D::ChangeRepresentation(SOV_RepresentationType rep) 
{
  if(!strcmp(m_ManagerInteractor->GetSelectedSO()->GetTypeName(),"GroupSpatialObject"))
    {
    itk::GroupSpatialObject<3>::ChildrenListType* children = m_ManagerInteractor->GetSelectedSO()->GetChildren(9999999);
    itk::GroupSpatialObject<3>::ChildrenListType::iterator it = children->begin();

    while(it!=children->end())
      {
      if(m_Renderer->GetVisualObject(*it))
        {
        m_Renderer->GetVisualObject(*it)->GetProperty()->SetRepresentationType(rep);
        }
      it++;
      }
    delete children;
    }
  else if(m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO()))
    {
    m_Renderer->GetVisualObject(m_ManagerInteractor->GetSelectedSO())->GetProperty()->SetRepresentationType(rep);
    }
  Display->Update();
}


void FltkSpatialObjectViewer3D::SetImageSlicer( ImageType::Pointer image )
{
  if( m_Image.IsNotNull() )
    {  
    m_Scene->RemoveSpatialObject( m_Image ); 
    }


  m_Image = ImageSpatialObjectType::New();
  m_Image->SetImage(image);

  // Set the image min/max
  typedef itk::MinimumMaximumImageFilter<ImageType> MinMaxType;
  MinMaxType::Pointer minMaxFilter = MinMaxType::New();
  minMaxFilter->SetInput(image);
  minMaxFilter->Update();
  IntensityMinBt->value(minMaxFilter->GetMinimum());
  IntensityMaxBt->value(minMaxFilter->GetMaximum());
  AddSpatialObjectToScene( m_Image );
  static_cast<sov::VisualImageProperty*>(m_Renderer->GetVisualObject(m_Image)->GetProperty())->SetIntensityRange(minMaxFilter->GetMinimum(),minMaxFilter->GetMaximum());
  static_cast<sov::VisualImageProperty*>(m_Renderer->GetVisualObject(m_Image)->GetProperty())->SetUseIntensityRange(true);
   
  ActivateSlider();
  Display->Update();
}


void FltkSpatialObjectViewer3D::AddSpatialObjectToScene( SpatialObjectType * object )
{
  m_Scene->AddSpatialObject( object );
  m_Manager->Update();
  std::cout << "Updating Display" << std::endl;
  Display->Update();
  std::cout << "Done" << std::endl;
  SpatialObjectType::ChildrenListType* children = object->GetChildren(999999);
  SpatialObjectType::ChildrenListType::const_iterator it = children->begin();

  while(it != children->end())
    {
    if(!strcmp((*it)->GetTypeName(),"DTITubeSpatialObject"))
      {
      m_Renderer->GetVisualObject(*it)->GetProperty()->SetUsePointColor(true);
      (*it)->Modified();
     // m_Renderer->AssociateWithRenderMethod(*it,"SurfaceNormalVectorVTKRenderMethod3D"); // Associate the object with a particular render method  
      }
    it++;
    }
  Display->Update();
/*
  
  it = children->begin();
  while(it != children->end())
    {

   if(!strcmp((*it)->GetTypeName(),"MeshSpatialObject"))
     {
      m_Renderer->GetVisualObject(*it)->GetProperty()->SetRepresentationType(SOV_POINTS); // Associate the object with a particular render method  
     }
    it++;

    i++;

    }

  Display->Update();
*/
/*

  it = children->begin();



  i=1;

  while(it != children->end())

    {

    std::cout << "Processing tube #" << i << " over " << children->size() << std::endl;

    (*it)->Modified();

    m_Renderer->AddToRenderMethod(*it,"CenterLineTubeVTKRenderMethod3D"); // Associate the object with a particular render method  

    it++;

    i++;

    }
*/
  delete children;
  m_Renderer->GetVTKRenderer()->ResetCamera();
}

void FltkSpatialObjectViewer3D::Update()
{
  Display->Update();
}

void FltkSpatialObjectViewer3D::SetRepresentationToSurface()
{
  //static_cast<sov::VisualTubeProperty*>(myRenderer3D->GetVisualObject("Tube1")->GetProperty())->SetRepresentationType(SOL_SURFACE);
  //Display4->Update();
}

void FltkSpatialObjectViewer3D::SetRepresentationToWireframe()
{
  //static_cast<sov::VisualTubeProperty*>(myRenderer3D->GetVisualObject("Tube1")->GetProperty())->SetRepresentationType(SOL_WIREFRAME);
  //Display4->Update();
}


void FltkSpatialObjectViewer3D::Show()
{
  window->show();
  //Fl::run();
}

void FltkSpatialObjectViewer3D::ActivateSlider()
{
  if( gui_xy->value() )
  { m_SliceDimension = 2; }
  else if( gui_yz->value() )
  { m_SliceDimension = 0; }
  else if( gui_xz->value() )
  { m_SliceDimension = 1;  }
  else { std::cout << " ERROR in viewer: please close and open again. " << std::endl; return;  }

  m_ImageDimensionSize = m_Image->GetImage()->GetLargestPossibleRegion().GetSize()[m_SliceDimension];
  slider->slider_size( 1/m_ImageDimensionSize ); 
  slider->value( m_Image->GetSlicePosition( m_SliceDimension ), 1, 0, m_ImageDimensionSize );


  slider->activate();
}


void FltkSpatialObjectViewer3D::Slide()
{
  float value = slider->value();
  m_Image->SetSlicePosition( m_SliceDimension, slider->value() );
  Update();
}

//Load object button callback 
void FltkSpatialObjectViewer3D::
LoadSpatialObject()
{
  const char * file = fl_file_chooser("Select Spatial Object", "*.*", ""); 
  if( file )
    {
    std::cout << "Loading Object (" << file << ")...";
    typedef itk::DefaultDynamicMeshTraits< float , 3, 3 > MeshTrait;
    typedef itk::SpatialObjectReader<3,float,MeshTrait>  ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( file );
    try
      {
      reader->Update();
      }
    catch( ... )
      {
      std::cout << "There was an error in specified filename" << std::endl;
      }

    std::cout << "Done." << std::endl;

    GroupSpatialObjectType::Pointer group = reader->GetGroup();
    
    group->ComputeObjectToWorldTransform(); 
    group->ComputeObjectToParentTransform(); 
    
    AddSpatialObjectToScene( group );
   m_ManagerInteractor->Select( group );
   }
}


//Load object button callback 
void FltkSpatialObjectViewer3D::
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
    SetImageSlicer( reader->GetOutput() );
    Update();
    }
  }

/** Update the intensity windowing */
void FltkSpatialObjectViewer3D::UpdateIWindowing()
{
  if(!m_Image)
    {
    return;
    }

  if( !m_Renderer->GetVisualObject(m_Image))
    {
    return;
    }

  static_cast<sov::VisualImageProperty*>(m_Renderer->GetVisualObject(m_Image)->GetProperty())->SetIntensityRange(IntensityMinBt->value(),IntensityMaxBt->value());
  Update();
}
