/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVisualObject.txx,v $
  Language:  C++
  Date:      $Date: 2006-03-16 21:58:33 $
  Version:   $Revision: 1.11 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _sovVisualObject_txx
#define _sovVisualObject_txx

#include "sovVisualObject.h"
#include "sovVisualArrowProperty.h"

#include <iostream>
#include <itkMesh.h>
#include <itkImageSpatialObject.h>
#include <itkMeshSpatialObject.h>
#include <vtkProp3D.h>

namespace sov
{

template < unsigned int TDimension >
VisualObject<TDimension>
::VisualObject()
{
  m_Property = 0;
  m_RenderMethodName = "";
  m_RenderMethod = NULL;
}

template < unsigned int TDimension >
VisualObject<TDimension>
::~VisualObject()
{
  m_MTime = 0;
  m_TransformMTime = 0;
  m_GlobalTransformMTime = 0;
  m_ObjectPropertyMTime = 0;
  m_PropertyAskRenderMethodToBeUpdatedTime = 0;
}


/** Set/Get the render method associated with the object */
template < unsigned int TDimension >
void
VisualObject<TDimension>
::SetRenderMethodName(std::string name)
{
  m_RenderMethodName = name;
  this->Modified();
}

template < unsigned int TDimension >
const char * 
VisualObject<TDimension>::GetRenderMethodName(void) const
{
  return m_RenderMethodName.c_str();
}

/** Set/Get the render method associated with the object */
template < unsigned int TDimension >
NDimensionalRenderMethod* 
VisualObject<TDimension>::GetRenderMethod(void) const
{
  return m_RenderMethod;
}

template < unsigned int TDimension >
void 
VisualObject<TDimension>::SetRenderMethod(NDimensionalRenderMethod* method)
{
  m_RenderMethod = method;
}

template < unsigned int TDimension >
void 
VisualObject<TDimension>
::SetObject(ObjectType * object)
{
  m_Object = object;

  // Create a propertytype 
  if(!strcmp(m_Object->GetTypeName(),"ImageSpatialObject"))
    {
    m_Property = new VisualImageProperty;
    static_cast<VisualImageProperty*>(m_Property)->SetPixelType(static_cast<ImageSpatialObject<TDimension,double>*>(object)->GetPixelType()); 
    }
  else if( (!strcmp(m_Object->GetTypeName(),"TubeSpatialObject"))
           || (!strcmp(m_Object->GetTypeName(),"VesselTubeSpatialObject"))
           || (!strcmp(m_Object->GetTypeName(),"DTITubeSpatialObject"))        
         )
    {
    m_Property = new VisualTubeProperty;
    }
  else if( (!strcmp(m_Object->GetTypeName(),"ArrowSpatialObject")) 
         )
    {
    m_Property = new VisualArrowProperty;
    }
 else if( (!strcmp(m_Object->GetTypeName(),"MeshSpatialObject"))
         )
    {
    m_Property = new VisualMeshProperty;
    typedef itk::Mesh<float,TDimension>  MeshType;
    static_cast<VisualMeshProperty*>(m_Property)->SetPixelType(static_cast<MeshSpatialObject<MeshType>*>(object)->GetPixelType()); 
    }
  else
    {
    m_Property = new PropertyType;
    }

  m_Property->SetName((char*)m_Object->GetProperty()->GetName().c_str());
  m_MTime = object->GetObjectMTime();
  m_TransformMTime = m_Object->GetTransformMTime();
  m_GlobalTransformMTime = m_Object->GetWorldTransformMTime();
  m_ObjectPropertyMTime = m_Object->GetProperty()->GetMTime();
  m_PropertyMTime =   m_Property->GetMTime();
  m_PropertyAskRenderMethodToBeUpdatedTime = m_Property->GetAskRenderMethodToBeUpdatedTime();
  m_PropertyAskRenderMethodToBeUpdatedTime--;
  m_TransformMTime--;
  m_GlobalTransformMTime--;
  if(m_MTime>0)
    {
    m_MTime--; // so we are sure that the object will be shown the first time
    }
}

template < unsigned int TDimension >
typename VisualObject<TDimension>::ObjectType*
VisualObject<TDimension>::GetObject()
{ 
  return m_Object;
}

/** Synchronize the time of both the object and properties 
 *  Only the render merthods should use this function when the
 *  visual object (i.e. the actor) is up to date on the screen 
 *  Since a Visual Object is passive, We force both the property
 *  and the Visual Object to have the same time as the object after the
 *  update, this means that the Modification time of the property could go 
 *  back in time after the update. */
template < unsigned int TDimension >
void 
VisualObject<TDimension>::UpdateMTime(void)
{
  m_MTime = m_Object->GetObjectMTime();
}

template < unsigned int TDimension >
void 
VisualObject<TDimension>::UpdateTransformMTime(void)
{
  m_TransformMTime = m_Object->GetWorldTransformMTime();
  m_GlobalTransformMTime = m_Object->GetWorldTransformMTime();
}

template < unsigned int TDimension >
void 
VisualObject<TDimension>::UpdateObjectPropertyMTime(void)
{
  m_ObjectPropertyMTime = m_Object->GetProperty()->GetMTime();
}

template < unsigned int TDimension >
void 
VisualObject<TDimension>::UpdatePropertyMTime(void)
{
  m_PropertyMTime = m_Property->GetMTime();
}

template < unsigned int TDimension >
void 
VisualObject<TDimension>::UpdatePropertyAskingForUpdateMTime(void)
{
  this->m_PropertyAskRenderMethodToBeUpdatedTime = m_Property->GetAskRenderMethodToBeUpdatedTime();
}

template < unsigned int TDimension >
bool 
VisualObject<TDimension>::IsModified(void)
{
  if(m_Object->GetObjectMTime()>m_MTime)
    {
    return true;
    }

  return false;
}

template < unsigned int TDimension >
bool 
VisualObject<TDimension>::IsTransformModified(void)
{
  if(m_TransformMTime < m_Object->GetTransformMTime())
    {
    m_Object->ComputeObjectToWorldTransform();
    }

  if(m_GlobalTransformMTime < m_Object->GetWorldTransformMTime())
    {
    return true;
    }
  return false;
}

template < unsigned int TDimension >
bool 
VisualObject<TDimension>::IsObjectPropertyModified(void)
{
  if(m_ObjectPropertyMTime < m_Object->GetProperty()->GetMTime())
    {
    return true;
    }
  return false;
}

template < unsigned int TDimension >
bool 
VisualObject<TDimension>::IsPropertyModified(void)
{
  if(m_Property->GetMTime()>m_PropertyMTime)
    {
    return true;
    }
  return false;
}


template < unsigned int TDimension >
bool 
VisualObject<TDimension>::IsPropertyAskingForUpdate()
{
  if(m_Property->GetAskRenderMethodToBeUpdatedTime()>m_PropertyAskRenderMethodToBeUpdatedTime)
    {
    m_PropertyAskRenderMethodToBeUpdatedTime = m_Property->GetAskRenderMethodToBeUpdatedTime();
    return true;
    }
  return false;
}


template < unsigned int TDimension >
void 
VisualObject<TDimension>::AddVTKDefined(void* pointer)
{
  m_VTKDefinedList.push_back(pointer);
  this->Modified();
}


template < unsigned int TDimension >
bool 
VisualObject<TDimension>::RemoveVTKDefined(void* pointer)
{
  VTKDefinedListType::iterator it = m_VTKDefinedList.begin();
  while(it != m_VTKDefinedList.end())
    {
    if(*it == pointer)
      {
      static_cast<vtkProp3D*>(*it)->Delete();
      m_VTKDefinedList.erase(it);
      return true;
      }
    it++;
    }
  return false;
  this->Modified();
}

template < unsigned int TDimension >
typename VisualObject<TDimension>::VTKDefinedListType 
VisualObject<TDimension>::GetVTKDefinedList(void)
{
  return m_VTKDefinedList;
}


} //end of namespace sov

#endif
