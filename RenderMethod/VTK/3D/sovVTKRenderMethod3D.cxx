/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-27 20:39:05 $
  Version:   $Revision: 1.8 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovVTKRenderMethod3D.h"
#include <itkSpatialObject.h>
#include <vtkMatrix4x4.h>
#include <vtkProperty.h>

namespace sov
{

VTKRenderMethod3D
::VTKRenderMethod3D()
{
  m_Type = "VTK3D";
  m_SupportedTypeList->clear();
}

VTKRenderMethod3D
::~VTKRenderMethod3D()
{
  /*std::list<vtkProp3D*>::iterator actor_it = m_ActorList.begin();
  while(actor_it != m_ActorList.end())
    {
    vtkProp3D* act = *actor_it;
    actor_it++;
    act->Delete();
    }
  m_ActorList.clear();*/
}

void
VTKRenderMethod3D
::PrintSelf (std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

void
VTKRenderMethod3D
::draw()
{
}

void 
VTKRenderMethod3D
::Update()
{
}

void 
VTKRenderMethod3D
::ClearActorToBeRemovedList(void)
{
  m_ActorToBeRemovedList.clear();
}

void 
VTKRenderMethod3D::RemoveVisualObject( VisualObjectType * object )
{
  Superclass::RemoveVisualObject(object);
  RemoveActor(object);
}

void
VTKRenderMethod3D::RemoveActor(VisualObjectType * object)
{
  std::list<void*> list = object->GetVTKDefinedList(); // remove all actors composing the VTK object
  std::list<void*>::iterator it = list.begin();
  
  std::list<vtkProp3D*>::iterator actor_it = m_ActorList.begin();
  while(actor_it != m_ActorList.end())
    {    
    std::list<void*> list = object->GetVTKDefinedList(); // remove all actors composing the VTK object
    std::list<void*>::iterator it = list.begin();
  
    bool removed = false;
    while(it != list.end())
      {
      if((*it) == *actor_it)
        { 
        object->RemoveVTKDefined(*it);
        removed = true;
        }
      it++;
      }
    if(removed)
      {
      m_ActorToBeRemovedList.push_back(*actor_it);
      std::list<vtkProp3D*>::iterator actor_it2 = actor_it;
      actor_it++;
      m_ActorList.erase(actor_it2);
      }
    else
      {
      actor_it++;
      }   
    }
}

bool 
VTKRenderMethod3D::IsSupported( const char* object ) const
{
  SupportedObjectTypeListType::const_iterator it = m_SupportedTypeList->begin();
  while(it != m_SupportedTypeList->end())
    {
    if(!strcmp(object,*it))
      {
      return true;
      }
    it++;
    }
  return false;
}

bool 
VTKRenderMethod3D::ActorIsCreated(vtkActor* actor)
{
  std::list<vtkProp3D*>::const_iterator it = m_ActorList.begin();
  bool exist = false;
  while(it != m_ActorList.end())
    {    
    if(actor == *it)
      {
      exist = true;
      }
    it++;
    }
  return exist;
}


 
void 
VTKRenderMethod3D::ApplyTransform(VisualObjectType * object)
{
  std::list<void*> list = object->GetVTKDefinedList(); // Get all the actors of the object
  std::list<void*>::iterator it = list.begin();

  //const double* scale = static_cast<itk::SpatialObject<3>*>(object->GetObject())->GetObjectToParentTransform()->GetScaleComponent();

  itk::Matrix<double,3,3> itkMatrix = static_cast<itk::SpatialObject<3>*>(object->GetObject())->GetObjectToWorldTransform()->GetMatrix();
  itk::Vector<double,3>   offset = static_cast<itk::SpatialObject<3>*>(object->GetObject())->GetObjectToWorldTransform()->GetOffset();
 
  vtkMatrix4x4* vtkMatrix = vtkMatrix4x4::New();

  for(unsigned int i=0;i<3;i++)
    {
    for(unsigned int j=0;j<3;j++)
      {
      vtkMatrix->SetElement(i,j,itkMatrix.GetVnlMatrix().get(i,j));   
      }
    vtkMatrix->SetElement(i,3,offset[i]);
    }

  while(it != list.end())
    {  
    static_cast<vtkProp3D*>(*it)->SetUserMatrix(vtkMatrix);
    //static_cast<vtkProp3D*>(*it)->SetScale(scale[0],scale[1],scale[2]);
    it++;
    }

  vtkMatrix->Delete();
}

void 
VTKRenderMethod3D::ChangeRepresentationType(VisualObjectType* object)
{
  std::list<void*> list = object->GetVTKDefinedList(); // Get all the actors of the object
  std::list<void*>::iterator it = list.begin();

  while(it != list.end())
    {  
    switch(object->GetProperty()->GetRepresentationType())
      {
       case SOV_WIREFRAME: static_cast<vtkActor*>(*it)->GetProperty()->SetRepresentationToWireframe();
                           break;
  
       case SOV_SURFACE: static_cast<vtkActor*>(*it)->GetProperty()->SetRepresentationToSurface();
                        break;
       case SOV_POINTS: static_cast<vtkActor*>(*it)->GetProperty()->SetRepresentationToPoints();
                        break;
          default:
       break;
      }
    it++;
    }
}

void 
VTKRenderMethod3D::ChangeProperty(VisualObjectType* object)
{
  if(!object)
    {
    std::cout << "ERROR: VTKRenderMethod3D::ChangeProperty(VisualObjectType* object): object is NULL!" << std::endl;
    return;
    }

  std::list<void*> list = object->GetVTKDefinedList(); // Get all the actors of the object
  std::list<void*>::iterator it = list.begin();

  while(it != list.end())
  {  
    // We change the color only if the RenderMethod allows it (i.e. global color for the object)
    if(!object->GetProperty()->GetTextureFilename()) // change the color only if there is no texture
    {
    if(!object->GetProperty()->GetUsePointColor())
      {
      static_cast<vtkActor*>(*it)->GetProperty()->SetColor(object->GetObject()->GetProperty()->GetColor().GetRed(),
        object->GetObject()->GetProperty()->GetColor().GetGreen(),
        object->GetObject()->GetProperty()->GetColor().GetBlue());
      static_cast<vtkActor*>(*it)->GetProperty()->SetOpacity(object->GetProperty()->GetOpacity());
      static_cast<vtkActor*>(*it)->GetProperty()->SetSpecular(object->GetProperty()->GetSpecular());
      static_cast<vtkActor*>(*it)->GetProperty()->SetSpecularPower(object->GetProperty()->GetSpecularPower());
      }
    }
  
    if(object->GetProperty()->GetVisibility())
      {
      static_cast<vtkProp*>(*it)->VisibilityOn();
      }
    else
      {
      static_cast<vtkProp*>(*it)->VisibilityOff();
      }
    it++;
    }
  
  if(!object->GetProperty()->GetTextureFilename()) 
    {
    this->ChangeRepresentationType(object);
    }
}


} // end of namespace sov
