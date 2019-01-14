/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPhantomRenderMethod.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:56 $
  Version:   $Revision: 1.4 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovPhantomRenderMethod.h"
#include <itkSpatialObject.h>
#include <vtkMatrix4x4.h>
#include <vtkProperty.h>

namespace sov
{

PhantomRenderMethod
::PhantomRenderMethod()
{
  m_Type = "Phantom";
  m_SupportedTypeList->clear();
}

PhantomRenderMethod
::~PhantomRenderMethod()
{
}

void
PhantomRenderMethod
::PrintSelf (std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

void 
PhantomRenderMethod
::Update()
{
}


void 
PhantomRenderMethod::RemoveVisualObject( VisualObjectType * object )
{
  Superclass::RemoveVisualObject(object);
//  RemoveActor(object);
}

void 
PhantomRenderMethod
::ClearActorToBeRemovedList(void)
{
  m_ActorToBeRemovedList.clear();
}

void
PhantomRenderMethod::RemoveActor(VisualObjectType * object)
{
  std::list<void*> list = object->GetVTKDefinedList(); // remove all actors composing the VTK object
  std::list<void*>::iterator it = list.begin();
  
  std::list<gwpTransform*>::iterator actor_it = m_ActorList.begin();
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
      std::list<gwpTransform*>::iterator actor_it2 = actor_it;
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
PhantomRenderMethod::IsSupported( const char* object ) const
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
 
void 
PhantomRenderMethod::ApplyTransform(VisualObjectType * object)
{
  std::list<void*> list = object->GetVTKDefinedList(); // Get all the actors of the object
  std::list<void*>::iterator it = list.begin();

  const double* scale = static_cast<itk::SpatialObject<3>*>(object->GetObject())->GetObjectToParentTransform()->GetScaleComponent();

  itk::Matrix<double,3,3> itkMatrix = static_cast<itk::SpatialObject<3>*>(object->GetObject())->GetObjectToWorldTransform()->GetMatrix();
  itk::Vector<double,3>   offset = static_cast<itk::SpatialObject<3>*>(object->GetObject())->GetObjectToWorldTransform()->GetOffset();
 
  gwpTransformMatrix gwpMatrix;

  gwpMatrix.setScale(scale[0],scale[1],scale[2]);
  gwpMatrix.setRotationMatrix(
    itkMatrix.GetVnlMatrix().get(0,0),itkMatrix.GetVnlMatrix().get(0,1),itkMatrix.GetVnlMatrix().get(0,2),
    itkMatrix.GetVnlMatrix().get(1,0),itkMatrix.GetVnlMatrix().get(1,1),itkMatrix.GetVnlMatrix().get(1,2),
    itkMatrix.GetVnlMatrix().get(2,0),itkMatrix.GetVnlMatrix().get(2,1),itkMatrix.GetVnlMatrix().get(2,2)
                             );   
  gwpMatrix.setTranslation(offset[0],offset[1],offset[2]);
  
  while(it != list.end())
  {  
    static_cast<gwpTransform*>(*it)->setTransformMatrix(gwpMatrix);
    it++;
  }
}


void 
PhantomRenderMethod::ChangeRepresentationType(VisualObjectType* object)
{
/*
  std::list<void*> list = object->GetVTKDefinedList(); // Get all the actors of the object
  std::list<void*>::iterator it = list.begin();

  while(it != list.end())
  {  
    switch(object->GetProperty()->GetRepresentationType())
    {
       case SOL_WIREFRAME: static_cast<vtkActor*>(*it)->GetProperty()->SetRepresentationToWireframe();
                           break;
  
       case SOL_SURFACE: static_cast<vtkActor*>(*it)->GetProperty()->SetRepresentationToSurface();
                        break;
       case SOL_POINTS: static_cast<vtkActor*>(*it)->GetProperty()->SetRepresentationToPoints();
                        break;
          default:
       break;
    }
    it++;
  }*/
}


void 
PhantomRenderMethod::ChangeProperty(VisualObjectType* object)
{
 /* std::list<void*> list = object->GetVTKDefinedList(); // Get all the actors of the object
  std::list<void*>::iterator it = list.begin();

  while(it != list.end())
  {  
    if(!object->GetProperty()->GetTextureFilename()) // change the color only if there is no texture
    {
      static_cast<vtkActor*>(*it)->GetProperty()->SetColor(object->GetObject()->GetProperty()->GetColor().GetRed(),
        object->GetObject()->GetProperty()->GetColor().GetGreen(),
        object->GetObject()->GetProperty()->GetColor().GetBlue());
      static_cast<vtkActor*>(*it)->GetProperty()->SetOpacity(object->GetProperty()->GetOpacity());

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
  }*/

}


} // end of namespace sov
