/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovRenderer.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-27 20:42:35 $
  Version:   $Revision: 1.7 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovRenderer.h"
#include <cstdio>

namespace sov
{

/** Constructor */
template < unsigned int TDimension >
Renderer< TDimension >
::Renderer()
{
  m_Id = GetNextAvailableId();
  sprintf(m_Name,"New Renderer %d",m_Id);
  m_Activated = true;
  m_Scene = NULL;
  m_RemoveUnusedRenderMethods = true;
}

/** Destructor */
template < unsigned int TDimension >
Renderer< TDimension >
::~Renderer()
{
 // We clean up
 typename MethodListType::const_iterator it = m_MethodList.begin();
 while(it != m_MethodList.end())
   {   
   std::list<VisualObjectType*> list = (*it)->GetVisualObjectList();
   typename VisualObjectListType::const_iterator vit = list.begin();

   while(vit!=list.end())
     {
     (*it)->RemoveVisualObject(*vit);
     this->RemoveObject(*vit);
     vit++;
     }
   it++;
   }
}

/** Add an object */
template < unsigned int TDimension >
void
Renderer< TDimension >
::AddObject( ObjectType * object, bool addChildren )
{
  if(strncmp(object->GetTypeName(),"Group",5))  // if the object is a group we do NOT add the object (multiple instances)
    {
    if(!this->ObjectExists(object))
      {
      // Add the object itself first
      VisualObjectType* visualObject = new VisualObjectType;
      m_VisualObjectList.push_back(visualObject);
      visualObject->SetObject(object);
      typename MethodType::Pointer method = UpdateMethodList( visualObject );
        if( method.GetPointer() )
        { 
        method->AddVisualObject(visualObject);
        }
      }
    }

  // Then add the children
  if(addChildren)
    {
    typename ObjectType::ChildrenListType* childrenList = object->GetChildren(0);
    typename ObjectType::ChildrenListType::const_iterator it 
                                                           = childrenList->begin();
    while(it != childrenList->end())
    {
      this->AddObject(*it);
      it++;
    }
    delete childrenList;
    }
}

/** Add an object and a render method at the same time */
template < unsigned int TDimension >
void
Renderer< TDimension >
::AddObject(ObjectType * object, MethodType * method )
{
  if( IsSupported(method) )
    {
    VisualObjectType* visualObject = new VisualObjectType;
    m_VisualObjectList.push_back(visualObject);
    visualObject->SetObject(object);
    AddRenderMethod(method);
    method->AddVisualObject(visualObject);
    }
}


/**  */
template < unsigned int TDimension >
void 
Renderer< TDimension >
::RemoveObject(VisualObjectType * object)
{
  m_VisualObjectList.remove(object);
  delete object;
  object = NULL;
}

/** */
template < unsigned int TDimension >
const unsigned int &
Renderer< TDimension >
::GetId() const
{
  return m_Id;
}

/** */
template < unsigned int TDimension >
void
Renderer< TDimension >
::draw()
{
  if( m_Activated )
  {
    typename MethodListType::iterator it,end;
    it = m_MethodList.begin();
    end = m_MethodList.end();
    for(; it != end; it++ )
    {   
      (*it)->draw();
    }
  }
  else
  {
    std::cout<<"Renderer not activated !!!"<<std::endl;
  }
}

/** Set the scene */
template < unsigned int TDimension >
void 
Renderer< TDimension >
::SetScene( SceneType * scene )
{
  m_Scene = scene;
  typename SceneType::ObjectListType * list = m_Scene->GetObjects(0);
  typename SceneType::ObjectListType::iterator it = list->begin();
  while(it != list->end())
  {
    AddObject(*it);
    it++;
  }

  m_NumberOfObjects = m_Scene->GetNumberOfObjects();

  delete list;
}

/** Get the scene */
template < unsigned int TDimension >
const typename Renderer< TDimension >::SceneType * 
Renderer< TDimension >
::GetScene() const
{
  return m_Scene;
}

/**  */
template < unsigned int TDimension >
void 
Renderer< TDimension >
::SetRenderMethodList( const MethodListType * list )
{
  m_MethodList = *list;
}

/**  */
template < unsigned int TDimension >
const typename Renderer< TDimension >::MethodListType *
Renderer< TDimension >
::GetRenderMethodList() const
{
  return & m_MethodList;
}

/**  */
template < unsigned int TDimension >
typename Renderer< TDimension >::MethodType *
Renderer< TDimension >
::UpdateMethodList( VisualObjectType* visualObject )
{

  VisualObjectProperty* visualProperty = visualObject->GetProperty();

  ObjectType* object = visualObject->GetObject();
  
  MethodPointer method = NULL;

  const char* pixelType = NULL;
  
  if(!strcmp(object->GetTypeName(),"ImageSpatialObject"))
    {
    pixelType =  static_cast<VisualImageProperty*>(visualProperty)->GetPixelType();
    }
 
  if(!strcmp(object->GetTypeName(),"MeshSpatialObject"))
    {
    pixelType =  static_cast<VisualMeshProperty*>(visualProperty)->GetPixelType(); 
    }

  if(visualObject->GetRenderMethod()) // if a type has been defined which means that the user knows what he wants
    {
    method = static_cast<typename Renderer< TDimension >::MethodType *>(visualObject->GetRenderMethod());
    }
  else
    {
    const char* methodName = visualObject->GetRenderMethodName();
    
    if(!strcmp(methodName,""))
      {
      methodName = NULL;
      }
   
    // Check in the list of the current render methods if one of the method
    // is able to display the object
    typename MethodListType::const_iterator it,end;
    it = m_MethodList.begin();
    end = m_MethodList.end();

    while(it != end)
      {
      if((*it)->IsObjectTypeSupported(object->GetTypeName()))
        {
        if(!strcmp(object->GetTypeName(),"ImageSpatialObject"))
          {
          if(!strcmp(pixelType,(*it)->GetPixelType()))
            {
              // check if the method name is the same
              if(methodName)
              {
                if(!strcmp((*it)->GetNameOfClass(),methodName))
                {
                  return(*it);
                }
              }
            }
          }
         else
          {

          // if no method name return the first method able to display the object
          // Factory will do the same thing so we don't need to go trhought the factory again and again
          if(!methodName)
            {
             return(*it);
            }
          // check if the method name is the same
          else
          {
            if(!strcmp((*it)->GetNameOfClass(),methodName))
            {
              return(*it);
            }
          }
          }
        }
      it++;
      }
 
    // else use the factory
    //method = RenderMethodFactory<TDimension>::CreateRenderMethod( object->GetTypeName(), 
    //                                       TDimension, this->GetType(), methodName, pixelType );

    method = GetRenderMethodViaFactory(object->GetTypeName(), 
                                       TDimension, 
                                       this->GetType(), 
                                       methodName,
                                       pixelType);

  }


  if(!method)
    {
    std::cout << "Warning: No render method found for: " << object->GetTypeName() << std::endl;
    return NULL;
    }

  // Check if the method doesn't exist already
  typename MethodListType::const_iterator it,end;
  it = m_MethodList.begin();
  end = m_MethodList.end();

  while(it != end)
    {
    if(!strcmp(method->GetNameOfClass(),(*it)->GetNameOfClass()))
      {
      if(!strcmp(object->GetTypeName(),"ImageSpatialObject"))
        {
        if(!strcmp(pixelType,(*it)->GetPixelType()))
          {
            return (*it);
          }
        }
      else
        {
        return (*it);
        }
      }
    it++;
    }


  // Add the new method to the method list
  if(method)
  {
    m_MethodList.push_back(method); 
  }

  return method;
}


/** Print the object properties */
template < unsigned int TDimension >
void
Renderer< TDimension >
::PrintSelf( std::ostream & os, Indent indent) const
{
  typename MethodListType::const_iterator it,end;
  it = m_MethodList.begin();
  end = m_MethodList.end();
  Superclass::PrintSelf(os,indent);
  os<<indent<<"Number of methods: "<<m_MethodList.size()<<std::endl;
  os<<indent<<"List of methods: ";  
  
  for(; it != end; it++)
  {
    os<<"["<<(*it)<<"]";
  }
  os<<std::endl;
}


/** Get the nuumber of objects */
template < unsigned int TDimension >
unsigned int
Renderer< TDimension >
::GetNumberOfObjects() const
{
  typename MethodListType::const_iterator it,end;
  it = m_MethodList.begin();
  end = m_MethodList.end();
  unsigned int n = 0;
  for(; it != end; it++ )
  {
    n += (*it)->GetNumberOfObjects();
  }
  return n;
}


/** Add a render method */
template < unsigned int TDimension >
void
Renderer< TDimension >
::AddRenderMethod( MethodType * method )
{
  method->SetWidth(m_Width);
  method->SetHeight(m_Height);
  m_MethodList.push_back(method);
}

/** Remove a render method */
template < unsigned int TDimension >
void 
Renderer< TDimension >
::RemoveRenderMethod( MethodType * method )
{
  m_MethodList.remove( method );
}

/** Get a render method */
template < unsigned int TDimension >
typename Renderer< TDimension >::MethodType * 
Renderer< TDimension >
::GetRenderMethod( const char * methodName ) const
{
  typename MethodListType::const_iterator it,end;
  std::string currentMethodName;
  
  it = m_MethodList.begin();
  end = m_MethodList.end();

  for(; it != end; it++ )
  {
    currentMethodName = (*it)->GetName();

    if( currentMethodName == methodName )
    {
      return (*it);
    }
  }
  return 0;
}


/** Active/Deactive a renderer */
template < unsigned int TDimension >
void
Renderer< TDimension >
::Active( bool activated )
{
  m_Activated = activated;
}

 
template < unsigned int TDimension >
bool
Renderer< TDimension >
::Active() const
{
  return m_Activated;
}
 
template < unsigned int TDimension >
const char * 
Renderer< TDimension >
::GetName( void ) const
{
  return m_Name;
}

template < unsigned int TDimension >
void 
Renderer< TDimension >
::SetName( const char * name )
{
  strcpy(m_Name,name);
}

template < unsigned int TDimension >
bool
Renderer< TDimension >
::IsSupported( MethodType* method ) const
{  
  if( m_Type == method->GetType() && TDimension == method->GetDimension() )
  {
    return true;
  }
  return false;
}


/** Return true if the visual object exists */
template < unsigned int TDimension >
bool
Renderer< TDimension >
::ObjectExists( ObjectType* object ) const
{  
  typename VisualObjectListType::const_iterator it = m_VisualObjectList.begin();

  while(it!=m_VisualObjectList.end())
  {
    if((*it)->GetObject() == object)
    {
      return true;
    }
    it++;
  }

  return false;
}

/** Return the number of visual object associated with an object */
template < unsigned int TDimension >
unsigned int
Renderer< TDimension >
::GetNumberOfVisualObjects( ObjectType* object ) const
{  
  unsigned int num = 0;
  typename VisualObjectListType::const_iterator it = m_VisualObjectList.begin();

  while(it!=m_VisualObjectList.end())
  {
    if((*it)->GetObject() == object)
    {
    num++;
    }
    it++;
  }

  return num;
}

/** Return true if an object is in the scene */
template < unsigned int TDimension >
bool
Renderer< TDimension >
::ObjectIsInScene( ObjectType* object ) const
{  
  typename SceneType::ObjectListType * list = m_Scene->GetObjects(9999);
  typename SceneType::ObjectListType::iterator it = list->begin();
  while(it != list->end())
    {       
    if(*it == object)
      {
      delete list;
      return true;
      }
    it++;
    }

  delete list;
  return false;
}


/** Update the pipeline */
template < unsigned int TDimension >
void Renderer< TDimension >
::Update( void )
{ 
  if(!m_Scene)
    {
    return;
    }

  /** Check if some object have been added*/
  if(m_NumberOfObjects < m_Scene->GetNumberOfObjects())
    {
    typename SceneType::ObjectListType * list = m_Scene->GetObjects();
    typename SceneType::ObjectListType::iterator it = list->begin();
    while(it != list->end())
      {       
      if(!ObjectExists(*it))
        {
        AddObject(*it,false); // we don't add children other objects are added two times
        }
      it++;
      }
    m_NumberOfObjects = m_Scene->GetNumberOfObjects();

    delete list;
    }
  
  /** Check if some object have been removed*/
  else if(m_NumberOfObjects > m_Scene->GetNumberOfObjects())
    {
    typename MethodListType::const_iterator it = m_MethodList.begin();
    for(; it != m_MethodList.end(); it++ )
      {   
      std::list<VisualObjectType*> list = (*it)->GetVisualObjectList();
      typename VisualObjectListType::const_iterator vit = list.begin();

      while(vit!=list.end())
        {
        if(!ObjectIsInScene((*vit)->GetObject()))
          {
          (*it)->RemoveVisualObject(*vit);
          RemoveObject(*vit);
          }
        vit++;
        }
      }
    m_NumberOfObjects = m_Scene->GetNumberOfObjects();
    }

  typename MethodListType::iterator it;
  it = m_MethodList.begin();
  for(; it != m_MethodList.end(); it++ )
    { 
    (*it)->Update();
    }
}

/** Get a Visual Object by name*/
template < unsigned int TDimension >
typename Renderer< TDimension >::VisualObjectType* 
Renderer< TDimension >
::GetVisualObject(const char * name) const
{
  typename VisualObjectListType::const_iterator it = m_VisualObjectList.begin();

  while(it!=m_VisualObjectList.end())
  {
    if(!strcmp((*it)->GetObject()->GetProperty()->GetName().c_str(),name))
    {
    return *it;
    }
    it++;
  }

  return NULL;
}

/** Get a Visual Object by object pointer */
template < unsigned int TDimension >
typename Renderer< TDimension >::VisualObjectType* 
Renderer< TDimension >
::GetVisualObject(ObjectType * object,unsigned int num) const
{
  unsigned int i=0;
  typename VisualObjectListType::const_iterator it = m_VisualObjectList.begin();

  while(it!=m_VisualObjectList.end())
    {
    if(object == (*it)->GetObject())
      {
      if(i==num)
        {
        return *it;
        }
      i++;
      }
    it++;
    }

  return NULL;
}

/** Update the display size */
template < unsigned int TDimension >
void
Renderer< TDimension >
::UpdateDisplaySize()
{
  typename MethodListType::iterator it,end;
  it = m_MethodList.begin();
  end = m_MethodList.end();

  for(; it != end; it++ )
  {
    (*it)->SetWidth(m_Width);
    (*it)->SetHeight(m_Height);
  }
}


/** Associate an object with a specific render method */
template < unsigned int TDimension >
void
Renderer< TDimension >
::AssociateWithRenderMethod(ObjectType* object, const char* methodName)
{
// if the object has already been created we remove it from the current render method and add it to the new one
 for(unsigned int i=0;i<this->GetNumberOfVisualObjects(object);i++)
  {
  // if the render method name are the same we continue
    if(!strcmp(this->GetVisualObject(object,i)->GetRenderMethodName(),methodName))
      {
      continue;
      }

    if(static_cast<RenderMethod<TDimension>*>(this->GetVisualObject(object,i)->GetRenderMethod()))
      {
      static_cast<RenderMethod<TDimension>*>(this->GetVisualObject(object,i)->GetRenderMethod())->RemoveVisualObject(this->GetVisualObject(object,i));
      this->Update();
      }
  
    // Check if the old render method is empty, if yes removed it
    if(m_RemoveUnusedRenderMethods)
      {
      typename MethodListType::iterator it = m_MethodList.begin();
      for(; it != m_MethodList.end(); it++ )
        {   
        if( (*it ==  this->GetVisualObject(object,i)->GetRenderMethod())
         &&
         ((*it)->GetNumberOfObjects() ==0)
        )
          {
          m_MethodList.erase(it);
          break;
          }
        }
    }
  }

  // Add the object itself first
  if(strncmp(object->GetTypeName(),"Group",5))  // if the object is a group we do NOT add the object (multiple instances)
    {
    object->Modified();
    VisualObjectType* visualObject = this->GetVisualObject(object);
    if(!this->ObjectExists(object))
      {
      visualObject = new VisualObjectType;
      m_VisualObjectList.push_back(visualObject);
      visualObject->SetObject(object);
      }


    visualObject->SetRenderMethodName(methodName);
    visualObject->SetRenderMethod(NULL);
    typename MethodType::Pointer method = UpdateMethodList( visualObject );
    if( method.GetPointer() )
      { 
      method->AddVisualObject(visualObject);
      }
    else
      {
      std::cout << "Renderer::AssociateWithRenderMethod(): No method defined for " <<  methodName << std::endl;
      }
    }
}


/** Associate an object with a specific render method */
template < unsigned int TDimension >
void
Renderer< TDimension >
::AssociateWithRenderMethod(ObjectType* object,  MethodType* method)
{
// if the object has already been created we remove it from the current render method and add it to the new one
  if(this->ObjectExists(object))
  {
    static_cast<RenderMethod<TDimension>*>(this->GetVisualObject(object)->GetRenderMethod())->RemoveVisualObject(this->GetVisualObject(object));
    
    // Check if the old render method is empty, if yes removed it
    if(m_RemoveUnusedRenderMethods)
    {
      typename MethodListType::iterator it = m_MethodList.begin();
      for(; it != m_MethodList.end(); it++ )
      {   
      if( (*it ==  this->GetVisualObject(object)->GetRenderMethod())
         &&
         ((*it)->GetNumberOfObjects() ==0)
        )
        {
        m_MethodList.remove(*it);
        break;
        }
      }
    }
  }

  // Add the method if not there already (i.e same pointer not same name)
  bool notfound = true;
  typename MethodListType::const_iterator it = m_MethodList.begin();
  for(; it != m_MethodList.end(); it++ )
    {   
    if(*it == method)
      {
      notfound = false;
      break;
      }
    }

  if(notfound)
    {
    this->AddRenderMethod(method);
    }


  // Add the object
  if(strncmp(object->GetTypeName(),"Group",5))  // if the object is a group we do NOT add the object (multiple instances)
    {
    VisualObjectType* visualObject = this->GetVisualObject(object);
    if(!this->ObjectExists(object))
      {
      visualObject = new VisualObjectType;
      m_VisualObjectList.push_back(visualObject);
      visualObject->SetObject(object);
      }
    visualObject->SetRenderMethodName(method->GetNameOfClass()); 
    visualObject->SetRenderMethod(method);  
    method->AddVisualObject(visualObject);
    }
}


/** Add an object to a specific render method */
template < unsigned int TDimension >
void
Renderer< TDimension >
::AddToRenderMethod(ObjectType* object, const char* methodName)
{
  // Add the object
  if(strncmp(object->GetTypeName(),"Group",5))  // if the object is a group we do NOT add the object (multiple instances)
    {
    VisualObjectType* visualObject2 = this->GetVisualObject(object);
    VisualObjectType* visualObject = new VisualObjectType;
    m_VisualObjectList.push_back(visualObject);
    visualObject->SetObject(object);
    visualObject->SetProperty(visualObject2->GetProperty()); // pass the visual property because they are shared
    visualObject->SetRenderMethodName(methodName);
    visualObject->SetRenderMethod(NULL);
    typename MethodType::Pointer method = UpdateMethodList( visualObject );
    if( method.GetPointer() )
      { 
      method->AddVisualObject(visualObject);
      }
    else
      {
      std::cout << "Renderer::AddToRenderMethod(): No method defined for " <<  methodName << std::endl;
      }
    }
}


/** Associate an object with a specific render method */
template < unsigned int TDimension >
void
Renderer< TDimension >
::AddToRenderMethod(ObjectType* object,  MethodType* method)
{
  // Add the method if not there already (i.e same pointer not same name)
  bool notfound = true;
  typename MethodListType::const_iterator it = m_MethodList.begin();
  for(; it != m_MethodList.end(); it++ )
    {   
    if(*it == method)
      {
      notfound = false;
      break;
      }
    }

  if(notfound)
    {
    this->AddRenderMethod(method);
    }


  // Add the object
  if(strncmp(object->GetTypeName(),"Group",5))  // if the object is a group we do NOT add the object (multiple instances)
    {
    VisualObjectType* visualObject2 = this->GetVisualObject(object);
    VisualObjectType* visualObject = new VisualObjectType;
    m_VisualObjectList.push_back(visualObject);
    visualObject->SetObject(object);
    visualObject->SetProperty(visualObject2->GetProperty()); // pass the visual property because they are shared
    visualObject->SetRenderMethodName(method->GetNameOfClass()); 
    visualObject->SetRenderMethod(method);  
    method->AddVisualObject(visualObject);
    }
}


} //end of namespace sov

