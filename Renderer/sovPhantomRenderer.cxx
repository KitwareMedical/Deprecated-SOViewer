/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPhantomRenderer.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:58 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovPhantomRenderer.h"

namespace sov
{

/** Constructor */
PhantomRenderer
::PhantomRenderer()
{
  m_Type = "Phantom";
  m_UpdateRendererOnly = false;
}

/** Destructor */
PhantomRenderer
::~PhantomRenderer()
{
}

void
PhantomRenderer
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}
  
void
PhantomRenderer
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
        MethodType* method = static_cast<PhantomRenderMethod *>((*it).GetPointer());
        std::list<gwpTransform*> actorList = method->GetActorList();
        std::list<gwpTransform*>::const_iterator it3 = actorList.begin();
        while(it3 != actorList.end())
          {
          gwpTransform* act = *it3;
          m_Phantom.GetRootSeparator()->addChild(act->AsTransform());
          it3++;
          }   
        }
      it2++;
      }
    it++;
    }
  }

void
PhantomRenderer
::AddObject(ObjectType * object, MethodType * method )
{
  Superclass::AddObject(object,method);
  std::list<gwpTransform*> actorList = method->GetActorList();
  std::list<gwpTransform*>::const_iterator it = actorList.begin();
  while(it != actorList.end())
  {
    gwpTransform* act = *it;
    m_Phantom.GetRootSeparator()->addChild(act->AsTransform());
    it++;
  }
}


/** Set the scene */
void 
PhantomRenderer
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

/** Start the Phantom */
void
PhantomRenderer
::Start()
{
  m_Phantom.Start();
}

/** Stop the Phantom */
void
PhantomRenderer
::Stop()
{
  m_Phantom.Stop();
}

/** Check if every render method has created a new actor and
 *  if so, add them to the VTK renderer */
void
PhantomRenderer
::Update()
{
  if(!m_UpdateRendererOnly)
    {
    Superclass::Update();
    }

  MethodListType::const_iterator method_it  = m_MethodList.begin();
  
  // Update added objects
  while(method_it != m_MethodList.end())
  {  

    std::list<gwpTransform*> actorToBeAddedList = static_cast<PhantomRenderMethod *>((*method_it).GetPointer())->GetActorToBeAddedList();
    std::list<gwpTransform*>::iterator it = actorToBeAddedList.begin();

    while(it != actorToBeAddedList.end())
    {
      gwpTransform* act = *it;
      m_Phantom.GetRootSeparator()->addChild(act->AsTransform());
      it++;
    }

    std::list<gwpTransform*> actorToBeRemovedList = static_cast<PhantomRenderMethod *>((*method_it).GetPointer())->GetActorToBeRemovedList();
    std::list<gwpTransform*>::iterator it2 = actorToBeRemovedList.begin();

    while(it2 != actorToBeRemovedList.end())
    {  
      gwpTransform* act = *it2;
      m_Phantom.GetRootSeparator()->removeChild(act->AsTransform());
      it2++;
    }

    static_cast<PhantomRenderMethod *>((*method_it).GetPointer())->ClearActorToBeRemovedList();
    static_cast<PhantomRenderMethod *>((*method_it).GetPointer())->ClearActorToBeAddedList();
 

    method_it++;
  }
}

} //end of namespace sov
