/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRendererSlicer.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-24 16:35:10 $
  Version:   $Revision: 1.8 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovVTKRendererSlicer.h"

#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkBMPWriter.h"
#include "vtkJPEGWriter.h"
#include <vtkAssemblyPath.h>
#include <vtkAssemblyNode.h>
#include <vtkPoints.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkWindowToImageFilter.h>
#include <vtkWindow.h>
#include <vtkClipDataSet.h>
#include <vtkSphere.h>
#include <vtkGeometryFilter.h>
#include <vtkCamera.h>

#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif

#ifdef WIN32
  #include <vtkWin32OpenGLRenderWindow.h>
#endif
 
namespace sov
{

/** Constructor */
VTKRendererSlicer
::VTKRendererSlicer()
{ 
  m_Interactor = vtkRenderWindowInteractor::New();
  m_InteractorStyle = vtkInteractorStyleTrackballCamera::New();
  m_Interactor->SetInteractorStyle(m_InteractorStyle);
  m_UpdateRendererOnly = false;
  m_CameraAttachedSpatialObject = NULL;
  m_CameraDirection.Fill(0);
  m_CameraRoll = 0;
  m_CameraOffset.Fill(0);
  m_Renderer->GetActiveCamera()->ParallelProjectionOn();
  this->m_Type = "VTKSlicer";
  this->SetOrientation(0);
  m_SliceNumber = 0;
  m_Depth = 0;
  m_Plane = vtkPlaneSource::New();
  m_Transform = TransformType::New();
}

/** Destructor */
VTKRendererSlicer
::~VTKRendererSlicer()
{
  m_Interactor->Delete();
  m_InteractorStyle->Delete();
  m_Plane->Delete();
  m_Transform->Delete();
}

void
VTKRendererSlicer
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}
  

/** Set the angles of the cutting plane */
void 
VTKRendererSlicer
::SetAngles(double alpha, double beta, double gamma)
{
  m_Transform->SetRotation(alpha,beta,gamma);
}

/** Set the offset of the center of the cutting plane */
void 
VTKRendererSlicer
::SetOffset(double x, double y, double z)
{
  TransformType::OffsetType offset;
  offset[0] = x;
  offset[1] = y;
  offset[2] = z;
  m_Transform->SetOffset(offset);
}

/**  Attach the camera to a specific object */
void 
VTKRendererSlicer
::AttachCamera(itk::SpatialObject<3>* object)
{
 m_CameraAttachedSpatialObject = object;
}


VisualObject<3>*
VTKRendererSlicer
::PickObject(float x,float y)
{
/*  #ifdef WIN32
    static_cast<vtkWin32OpenGLRenderWindow*>(m_Renderer->GetRenderWindow())->MakeCurrent();
    static_cast<vtkWin32OpenGLRenderWindow*>(m_Renderer->GetRenderWindow())->UseWithOtherWindow(false);
  #endif
*/   
   vtkAssemblyPath* pickedCollection = m_Renderer->PickProp(x,y);
   
/*  #ifdef WIN32
    static_cast<vtkWin32OpenGLRenderWindow*>(m_Renderer->GetRenderWindow())->UseWithOtherWindow(true);
  #endif
*/

  if(pickedCollection != 0)
  {
#if(VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 4))
    vtkProp * actor = pickedCollection->GetFirstNode()->GetViewProp();
#else
    vtkProp * actor = pickedCollection->GetFirstNode()->GetProp();
#endif

    VisualObjectListType::const_iterator it = this->m_VisualObjectList.begin();

    while(it!=this->m_VisualObjectList.end())
    {
      std::list<void*> list = (*it)->GetVTKDefinedList(); // Get all the actors of the object
      std::list<void*>::iterator it2 = list.begin();

      while(it2 != list.end())
      {  
        if((*it2) == actor)
        {
          return (*it);
        }
        it2++;
      }
      it++;
    }

    pickedCollection->Delete();

  }

  return NULL;
}


/** Return a 3D point Picked in the scene */
VTKRendererSlicer::PointType
VTKRendererSlicer
::PickPoint(float x,float y)
{

  PointType point;
  point.Fill(0);

/*  #ifdef WIN32
    static_cast<vtkWin32OpenGLRenderWindow*>(m_Renderer->GetRenderWindow())->MakeCurrent();
    static_cast<vtkWin32OpenGLRenderWindow*>(m_Renderer->GetRenderWindow())->UseWithOtherWindow(false);
  #endif
 */  
  vtkPicker* picker = vtkPicker::New();
  if( picker->Pick(x,y,0,m_Renderer)  )
  {
    vtkFloatingPointType* pt = picker->GetPickedPositions()->GetPoint(0);
    for(unsigned int i=0;i<3;i++)
    {
      point[i]=(float)pt[i];
    }
  }

  picker->Delete();

/*  #ifdef WIN32
    static_cast<vtkWin32OpenGLRenderWindow*>(m_Renderer->GetRenderWindow())->UseWithOtherWindow(true);
  #endif
*/
  return point;
}


void 
VTKRendererSlicer
::AutoZoom(ObjectType* object)
{
  // We need at least an image
  if(strcmp(object->GetTypeName(),"ImageSpatialObject"))
    {
    return;
    }

  unsigned char widthAxis,heightAxis,depthAxis;

   switch(this->m_Orientation)
      {
      case 0:
        widthAxis = 1;
        heightAxis = 2;
        depthAxis = 0;
        break;
      case 1:
        widthAxis = 0;
        heightAxis = 2;
        depthAxis = 1;
        break;
      case 2:
        widthAxis = 0;
        heightAxis = 1;
        depthAxis = 2;
        break;
      }

  
 
  double zoom = static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetLargestPossibleRegion().GetSize()[heightAxis];
  double spacing = static_cast<itk::ImageSpatialObject<3>*>(object)->GetImage()->GetSpacing()[heightAxis];
  m_Renderer->GetActiveCamera()->SetParallelScale(((((zoom/2.0)-0.5)*spacing)));
 
  MethodListType::iterator it, end;
  it = m_MethodList.begin();
  end = m_MethodList.end();
  for(; it != end; it++ )
    {
    dynamic_cast<MethodType*>((*it).GetPointer())->
      SetOrientation(m_Orientation);
    dynamic_cast<MethodType*>((*it).GetPointer())->
      SetPlane(m_Plane);
    dynamic_cast<MethodType*>((*it).GetPointer())->
      SetTransform(m_Transform);
    }
}

/** Set depth */
void VTKRendererSlicer::SetDepth(double depth)
{
  m_Depth = depth;
  MethodListType::iterator it, end;
  it = m_MethodList.begin();
  end = m_MethodList.end();
  for(; it != end; it++ )
    {
    dynamic_cast<MethodType*>((*it).GetPointer())->
      SetDepth(depth);
    }
}

/** Set the orientation of the slice to be viewed 
 *  The Scene should be set as well as the image for this function to work */
void VTKRendererSlicer
::SetOrientation(unsigned int orientation)
{
  m_Orientation = orientation;
  
  MethodListType::iterator it, end;
  it = m_MethodList.begin();
  end = m_MethodList.end();
  for(; it != end; it++ )
  {
    dynamic_cast<MethodType*>((*it).GetPointer())->
      SetOrientation(orientation);
  }

  VisualObjectListType::iterator it2 = m_VisualObjectList.begin();

  while(it2 != m_VisualObjectList.end())
    {
    if(!strcmp((*it2)->GetObject()->GetTypeName(),"ImageSpatialObject"))
      {
      static_cast<VisualImageProperty*>((*it2)->GetProperty())->SetOrientation(this->m_Orientation);     
      //static_cast<itk::ImageSpatialObject<3>*>((*it2)->GetObject())->SetSlicePosition(this->m_Orientation,this->m_SliceNumber);
      
      // We define the size of the plane
     unsigned int widthAxis,heightAxis;//,depthAxis;
     switch(this->m_Orientation)
      {
      case 0:
        widthAxis = 1;
        heightAxis = 2;
        //depthAxis = 0;
        break;
      case 1:
        widthAxis = 0;
        heightAxis = 2;
        //depthAxis = 1;
        break;
      case 2:
        widthAxis = 0;
        heightAxis = 1;
        //depthAxis = 2;
        break;
      }
      double origin[2];
      origin[0] = -((double)((*it2)->GetObject()->GetBoundingBox()->GetBounds()[2*widthAxis+1])/2.0)+0.5;
      origin[1] = -((double)((*it2)->GetObject()->GetBoundingBox()->GetBounds()[2*heightAxis+1])/2.0)+0.5;
      m_Plane->SetOrigin(origin[0],origin[1],0);
      m_Plane->SetPoint1(origin[0]+(double)(*it2)->GetObject()->GetBoundingBox()->GetBounds()[2*widthAxis+1]-1, origin[1],0);
      m_Plane->SetPoint2(origin[0], origin[1]+(double)(*it2)->GetObject()->GetBoundingBox()->GetBounds()[2*heightAxis+1]-1,0);

      // we set the center of rotation as the center of the image
      itk::Point<double,3> cor;
      cor[0] = ((*it2)->GetObject()->GetBoundingBox()->GetBounds()[1]-(*it2)->GetObject()->GetBoundingBox()->GetBounds()[0])/2;
      cor[1] = ((*it2)->GetObject()->GetBoundingBox()->GetBounds()[3]-(*it2)->GetObject()->GetBoundingBox()->GetBounds()[2])/2;
      cor[2] = ((*it2)->GetObject()->GetBoundingBox()->GetBounds()[5]-(*it2)->GetObject()->GetBoundingBox()->GetBounds()[4])/2;

      m_Transform->SetCenter(cor);
      AutoZoom((*it2)->GetObject());
      }
    it2++;
    }

}

void
VTKRendererSlicer
::AddObject(ObjectType* object,bool val)
{
  Superclass::AddObject(object,val);
  this->AutoZoom(object);
}

void
VTKRendererSlicer
::AddObject( ObjectType * object )
  { 
  Superclass::AddObject(object);
  this->AutoZoom(object);
 
  
  m_Renderer->GetActiveCamera()->Zoom((1.0/49.5));

  
  MethodListType::const_iterator it =  m_MethodList.begin();
  while(it != m_MethodList.end())
    {
    
    const std::list<VisualObjectType *>* visualObjectList = (*it)->GetObjectList();
    std::list<VisualObjectType *>::const_iterator it2 = visualObjectList->begin();
    
    while(it2 != visualObjectList->end())
      {
      if((*it2)->GetObject() == object)
        {
        MethodType* method = static_cast<VTKRenderMethodSlicer *>((*it).GetPointer());
        std::list<vtkProp3D*> actorList = method->GetActorList();
        std::list<vtkProp3D*>::const_iterator it3 = actorList.begin();
        while(it3 != actorList.end())
          {
          vtkProp3D* act = *it3;
          m_Renderer->AddActor(act);
          it3++;
          }   
        }
      it2++;
      }
    it++;
    }
  }

void
VTKRendererSlicer
::AddObject(ObjectType * object, SuperclassMethodType * method )
{
  Superclass::AddObject(object,method);
  this->AutoZoom(object);

  std::list<vtkProp3D*> actorList = static_cast<MethodType*>(method)->GetActorList();
  std::list<vtkProp3D*>::const_iterator it = actorList.begin();
  while(it != actorList.end())
  {
    vtkProp3D* act = *it;
    m_Renderer->AddActor(act);
    //act->Delete();  
    it++;
  }
}


/** Set the scene */
void 
VTKRendererSlicer
::SetScene( SceneType * scene )
{
  m_Scene = scene;
  SceneType::ObjectListType * list = m_Scene->GetObjects(0);
  SceneType::ObjectListType::iterator it = list->begin();
  SceneType::ObjectListType::iterator itEnd = list->end();
  while(it != itEnd)
  {
    this->AddObject(*it);
    it++;
  }
  delete list;

  m_NumberOfObjects = m_Scene->GetNumberOfObjects();
}

/** Check if every render method has created a new actor and
 *  if so, add them to the VTK renderer */
void
VTKRendererSlicer
::Update()
{
  // If the camera is attached to a spatial object we update the position of the camera
  if(m_CameraAttachedSpatialObject)
    {
    vtkCamera* cam = m_Renderer->GetActiveCamera();
    itk::Vector<double,3> offset = m_CameraAttachedSpatialObject->GetObjectToWorldTransform()->GetOffset();
    cam->SetFocalPoint(offset[0]+m_CameraDirection[0],offset[1]+m_CameraDirection[1],offset[2]+m_CameraDirection[2]);
    cam->SetPosition(offset[0]+m_CameraOffset[0],offset[1]+m_CameraOffset[1],offset[2]+m_CameraOffset[2]);
    cam->ComputeViewPlaneNormal();
    cam->SetViewUp(1,0,0);
    cam->OrthogonalizeViewUp();
    cam->Roll(m_CameraRoll);
    //cam->Zoom(0.5);
    //cam->ParallelProjectionOn();
    //m_Renderer->ResetCamera();
    }

  if(!m_UpdateRendererOnly)
    {
    Superclass::Update();
    }
  MethodListType::const_iterator method_it  = m_MethodList.begin();
  
  // Update added objects
  while(method_it != m_MethodList.end())
  {  

    std::list<vtkProp3D*> actorToBeAddedList = static_cast<VTKRenderMethodSlicer *>((*method_it).GetPointer())->GetActorToBeAddedList();
    std::list<vtkProp3D*>::iterator it = actorToBeAddedList.begin();

    while(it != actorToBeAddedList.end())
    {
    //std::cout << "Adding!" << std::endl;
      vtkProp3D* act = *it;
      m_Renderer->AddActor(act);
      //act->Delete();  
      it++;
    }


    std::list<vtkProp3D*> actorToBeRemovedList = static_cast<VTKRenderMethodSlicer *>((*method_it).GetPointer())->GetActorToBeRemovedList();
    std::list<vtkProp3D*>::iterator it2 = actorToBeRemovedList.begin();

    while(it2 != actorToBeRemovedList.end())
    {  
      vtkProp3D* act = *it2;
      m_Renderer->RemoveActor(act);
      //act->Delete(); 
      it2++;
    }

    static_cast<VTKRenderMethodSlicer *>((*method_it).GetPointer())->ClearActorToBeRemovedList();
    static_cast<VTKRenderMethodSlicer *>((*method_it).GetPointer())->ClearActorToBeAddedList();
 

    method_it++;
  }
}

/** 
 * Used to change between, e.g., Joystick and Trackball mode.  The
 * style must be created by the appropriate ::New(), but will be
 * deleted by the destructor of the VTKRendererSlicer object.  The
 * previously set style is automatically deleted.
 */
void 
VTKRendererSlicer::
SetVTKInteractorStyle(vtkInteractorStyle *style)
{
  m_Interactor->SetInteractorStyle(style);
  m_InteractorStyle->Delete();
  m_InteractorStyle = style;
}

/** Save the Render as a BMP image */
void 
VTKRendererSlicer
::SaveRenderAsBMP(const char* name)
{
  vtkWindowToImageFilter* exporter = vtkWindowToImageFilter::New();
  exporter->SetInput(m_Renderer->GetVTKWindow());
  exporter->Update();

  vtkBMPWriter* writer = vtkBMPWriter::New();
  writer->SetInput(exporter->GetOutput());
  writer->SetFileName(name);
  writer->Write();
  exporter->Delete();
  writer->Delete();
}

/** Save the Render as a JPEG image */
void 
VTKRendererSlicer
::SaveRenderAsJPEG(const char* name)
{
  vtkWindowToImageFilter* exporter = vtkWindowToImageFilter::New();
  exporter->SetInput(m_Renderer->GetVTKWindow());
  exporter->Update();

  vtkJPEGWriter* writer = vtkJPEGWriter::New();
  writer->SetInput(exporter->GetOutput());
  writer->SetFileName(name);
  writer->Write();
  exporter->Delete();
  writer->Delete();
}




} //end of namespace sov
