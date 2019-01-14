/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRenderer3D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-27 20:40:07 $
  Version:   $Revision: 1.11 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovVTKRenderer3D.h"

#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkBMPWriter.h"
#include <vtkPNGWriter.h>
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
VTKRenderer3D
::VTKRenderer3D()
{ 
  m_Interactor = vtkRenderWindowInteractor::New();
  m_InteractorStyle = vtkInteractorStyleTrackballCamera::New();
  m_Interactor->SetInteractorStyle(m_InteractorStyle);
  m_UpdateRendererOnly = false;
  m_CameraAttachedSpatialObject = NULL;
  m_CameraDirection.Fill(0);
  m_CameraRoll = 0;
  m_CameraOffset.Fill(0);
  this->m_Type = "VTK3D";
}

/** Destructor */
VTKRenderer3D
::~VTKRenderer3D()
{ 
  m_Interactor->Delete();
  m_InteractorStyle->Delete();
}

void
VTKRenderer3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}
  

/**  Attach the camera to a specific object */
void 
VTKRenderer3D
::AttachCamera(itk::SpatialObject<3>* object)
{
 m_CameraAttachedSpatialObject = object;
}

/** For the moment we can only cut the scene with an EllipseSpatialObject */
void
VTKRenderer3D
::CutObject(itk::EllipseSpatialObject<3>* object)
{
  if(object == NULL)
    {
    return;
    }

  vtkSphere* sphere = vtkSphere::New();
  sphere->SetRadius(object->GetRadius()[0]*object->GetObjectToWorldTransform()->GetScaleComponent()[0]);
  itk::Vector<double,3> offset;
  offset = object->GetObjectToWorldTransform()->GetOffset();
  sphere->SetCenter(offset[0],offset[1],offset[2]);

  vtkClipDataSet*  clipper = vtkClipDataSet::New();
  
  SceneType::ObjectListType* objects = this->m_Scene->GetObjects(99999);
  SceneType::ObjectListType::const_iterator itObj = objects->begin();
  
  while(itObj != objects->end())
    {
    if(*itObj != object)
      {

      if(!this->GetVisualObject(*itObj))
        {
        std::cout << "Cannot GetVisualObject" << std::endl;
        itObj++;
        continue;
        }

      if(!this->GetVisualObject(*itObj)->GetProperty())
        {
        std::cout << "Cannot GetProperty" << std::endl;
        itObj++;
        continue;
        }

      std::list<void*> list = this->GetVisualObject(*itObj)->GetVTKDefinedList(); // Get all the actors of the object
      std::list<void*>::iterator it = list.begin();

      //std::list<vtkProp*> newList;

      MethodListType::iterator method_it  = this->m_MethodList.begin(); // take the first render method to add actors (just a little trick)
      

      while(it != list.end())
        {
        std::cout << "Mapper = " << static_cast<vtkActor*>(*it)->GetMapper() << std::endl;
        std::cout << "Input = " << static_cast<vtkActor*>(*it)->GetMapper()->GetInput() << std::endl;
        clipper->SetInput(static_cast<vtkActor*>(*it)->GetMapper()->GetInput());
        clipper->SetClipFunction(sphere);
        clipper->GenerateClipScalarsOn();
        clipper->GenerateClippedOutputOn();
        clipper->InsideOutOn();
        clipper->SetValue(0.5);
        clipper->Update();

        //vtkGeometryFilter* gf = vtkGeometryFilter::New();
        //gf->SetInput(clipper->GetOutput());
        //gf->Update();
        /*vtkPolyDataMapper* clipMapper = vtkPolyDataMapper::New();
        clipMapper->SetInput(gf->GetOutput());
        vtkActor* clipActor = vtkActor::New();
        clipActor->SetMapper(clipMapper);*/
        //static_cast<VTKRenderMethod3D *>((*method_it).GetPointer())->GetActorToBeAddedList().push_back(clipActor);
        //static_cast<VTKRenderMethod3D *>((*method_it).GetPointer())->GetActorToBeRemovedList().push_back(static_cast<vtkActor*>(*it));      
        //m_Renderer->AddActor(clipActor);
        //m_Renderer->RemoveActor(static_cast<vtkProp*>(*it));
        //*it = clipActor;
        it++;
        }
        this->Update();



      }
    itObj++;
    }
  delete objects;
  sphere->Delete();
  clipper->Delete();
}


VisualObject<3>*
VTKRenderer3D
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
VTKRenderer3D::PointType
VTKRenderer3D
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
VTKRenderer3D
::AddObject(ObjectType* object,bool val)
{
  Superclass::AddObject(object,val);
}

void
VTKRenderer3D
::AddObject( ObjectType * object )
  { 
  Superclass::AddObject(object);
  
  MethodListType::const_iterator it =  m_MethodList.begin();
  while(it != m_MethodList.end())
    {
    
    const std::list<VisualObjectType *>* visualObjectList = (*it)->GetObjectList();
    std::list<VisualObjectType *>::const_iterator it2 = visualObjectList->begin();
    
    while(it2 != visualObjectList->end())
      {
      if((*it2)->GetObject() == object)
        {
        MethodType* method = static_cast<VTKRenderMethod3D *>((*it).GetPointer());
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
VTKRenderer3D
::AddObject(ObjectType * object, SuperclassMethodType * method )
{
  Superclass::AddObject(object,method);

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
void VTKRenderer3D
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
void VTKRenderer3D
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
    }

  if(!m_UpdateRendererOnly)
    {
    Superclass::Update();
    }

  MethodListType::const_iterator method_it  = m_MethodList.begin();

  // Update added objects
  while(method_it != m_MethodList.end())
    {  
    std::list<vtkProp3D*> actorToBeAddedList = static_cast<VTKRenderMethod3D *>((*method_it).GetPointer())->GetActorToBeAddedList();
    std::list<vtkProp3D*>::iterator it = actorToBeAddedList.begin();

    while(it != actorToBeAddedList.end())
      {
      vtkProp3D* act = *it;
      m_Renderer->AddActor(act);
      it++;
      }
    std::list<vtkProp3D*> actorToBeRemovedList = static_cast<VTKRenderMethod3D *>((*method_it).GetPointer())->GetActorToBeRemovedList();
    std::list<vtkProp3D*>::iterator it2 = actorToBeRemovedList.begin();

    while(it2 != actorToBeRemovedList.end())
      {  
      vtkProp3D* act = *it2;
      m_Renderer->RemoveActor(act);
      //act->Delete();
      it2++;
      }

    static_cast<VTKRenderMethod3D *>((*method_it).GetPointer())->ClearActorToBeRemovedList();
    static_cast<VTKRenderMethod3D *>((*method_it).GetPointer())->ClearActorToBeAddedList();
    
    method_it++;
  }
}

/** 
 * Used to change between, e.g., Joystick and Trackball mode.  The
 * style must be created by the appropriate ::New(), but will be
 * deleted by the destructor of the VTKRenderer3D object.  The
 * previously set style is automatically deleted.
 */
void 
VTKRenderer3D::
SetVTKInteractorStyle(vtkInteractorStyle *style)
{
  m_Interactor->SetInteractorStyle(style);
  m_InteractorStyle->Delete();
  m_InteractorStyle = style;
}

/** Save the Render as a BMP image */
void 
VTKRenderer3D
::SaveRenderAsBMP(const char* name)
{
  vtkWindowToImageFilter* exporter = vtkWindowToImageFilter::New();
  exporter->SetInput(m_Renderer->GetVTKWindow());
  exporter->Update();

  vtkBMPWriter* writer = vtkBMPWriter::New();
  writer->SetInput(exporter->GetOutput());
  writer->SetFileName(name);
  writer->Write();
  writer->Delete();
  exporter->Delete();
}

/** Save the Render as a JPEG image */
void 
VTKRenderer3D
::SaveRenderAsJPEG(const char* name)
{
  vtkWindowToImageFilter* exporter = vtkWindowToImageFilter::New();
  exporter->SetInput(m_Renderer->GetVTKWindow());
  exporter->Update();

  vtkJPEGWriter* writer = vtkJPEGWriter::New();
  writer->SetInput(exporter->GetOutput());
  writer->SetFileName(name);
  writer->Write();
  writer->Delete();
  exporter->Delete();
}


/** Save the Render as a PNG image */
void 
VTKRenderer3D
::SaveRenderAsPNG(const char* name)
{
  vtkWindowToImageFilter* exporter = vtkWindowToImageFilter::New();
  exporter->SetInput(m_Renderer->GetVTKWindow());
  exporter->Update();

  vtkPNGWriter* writer = vtkPNGWriter::New();
  writer->SetInput(exporter->GetOutput());
  writer->SetFileName(name);
  writer->Write();
  writer->Delete();
  exporter->Delete();
}


} //end of namespace sov
