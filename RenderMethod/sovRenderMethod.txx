/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovRenderMethod.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-01 21:24:35 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovRenderMethod.h"

namespace sov
{

/** Constructor */ 
template< unsigned int TDimension >
RenderMethod< TDimension >
::RenderMethod()
{
  //m_ObjectList = new ObjectListType();
  m_SupportedTypeList = new SupportedObjectTypeListType();
}


/** Destructor */
template< unsigned int TDimension >
RenderMethod< TDimension >
::~RenderMethod()
{
}

/** Add an object to the list of objects to render */
template< unsigned int TDimension >
void 
RenderMethod< TDimension >
::AddVisualObject( VisualObjectType * object )
{
  object->SetRenderMethod(this);
  m_ObjectList.push_back(object);
  m_ObjectList.unique();
}


/** Remove an object from the list of objects to render */
template< unsigned int TDimension >
void
RenderMethod< TDimension >
::RemoveVisualObject( VisualObjectType * object )
{
  m_ObjectList.remove(object);
  //Update();
}

/** Set the list of objects to render */
template< unsigned int TDimension >
void 
RenderMethod< TDimension >
::SetObjectList( ObjectListType * list )
{
  m_ObjectList = *list;
  Update();
}

/** Get the list of objects to render */
template< unsigned int TDimension >
const typename RenderMethod< TDimension >::ObjectListType * 
RenderMethod< TDimension >
::GetObjectList() const
{
  return &m_ObjectList;
}

/** Get the number of objects to render */
template< unsigned int TDimension >
unsigned int 
RenderMethod< TDimension >
::GetNumberOfObjects() const
{
  return m_ObjectList.size();
}

template< unsigned int TDimension >
const typename RenderMethod< TDimension >::ObjectType *
RenderMethod< TDimension >
::GetObject( const char * objectName ) const
{
  typename ObjectListType::const_iterator it,end;
  it = m_ObjectList.begin();
  end = m_ObjectList.end();
  for(; it != end; it++ )
  {
    if( (*it)->GetProperty()->GetName() == objectName )
    {
      return (*it)->GetObject();
    }
  }
  return 0;
}

  
template< unsigned int TDimension >
const typename RenderMethod< TDimension >::ObjectType *
RenderMethod< TDimension >
::GetConstObject( const char * objectName ) const
{
  const ObjectType * object = GetObject(objectName);
  return (const ObjectType *)object;
}
 
/** Print out the object in a stream */
template< unsigned int TDimension >
void
RenderMethod< TDimension >
::PrintSelf (std::ostream &os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<indent<<"Name: "<<m_Name<<std::endl;
  os<<indent<<"Number of objects: "<<m_ObjectList.size()<<std::endl;
  os<<indent<<"Number of supported types: "<<m_SupportedTypeList->size()<<std::endl;
}


/** Returns true if the type passed as argument
 *  is supported, false otherwise */
template< unsigned int TDimension >
bool 
RenderMethod< TDimension >
::IsObjectTypeSupported( const char * type ) const
{
  SupportedObjectTypeListType::iterator it;
  it = FindValue( type );
  if( it == m_SupportedTypeList->end() )
  {
    return false;
  }
  return true;
}

/** Add a supported type to the render method */
template< unsigned int TDimension >
void 
RenderMethod< TDimension >
::AddSupportedType( const char * type )
{
  m_SupportedTypeList->push_back( type );
}


/** Remove a supported type from the list */
template< unsigned int TDimension >
void 
RenderMethod< TDimension >
::RemoveSupportedType( const char * type )
{
  m_SupportedTypeList->remove( type );
}

/** Find a value into the list of supported types
 *  and returns a pointer to it */
template< unsigned int TDimension >
RenderMethod< TDimension >::SupportedObjectTypeListType::iterator
RenderMethod< TDimension >
::FindValue( const char * type ) const
{
  SupportedObjectTypeListType::iterator it,end;
  it = m_SupportedTypeList->begin();
  end = m_SupportedTypeList->end();
  std::string s;
  for(; it != end; it++ )
  {
    s = (*it);
    if( s == type )
    {
      return it;
    }
  }
  return end; 
}

template< unsigned int TDimension >
bool 
RenderMethod< TDimension >
::ContainsObject( const char * objectName ) const
{
  ObjectType                * currentObject;
  std::string               currentObjectName;
  typename ObjectListType::const_iterator  it,end;
  
  it = m_ObjectList.begin();
  end = m_ObjectList.end();

  for(; it != end; it++ )
  {
    currentObject = (ObjectType *)(*it);
    currentObjectName = currentObject->GetProperty()->GetName();

    if( currentObjectName == objectName )
    {
      return true;
    }
  }
  return false;
}

template< unsigned int TDimension >
unsigned long 
RenderMethod< TDimension >
::GetMTime( void ) const
{
  unsigned long latestTime,localTime;
  typename ObjectListType::const_iterator  it,end;
  it = m_ObjectList.begin();
  end = m_ObjectList.end();

  latestTime = Object::GetMTime();
  
  for(; it != end; it++ )
  {
    localTime = (*it)->GetObject()->GetMTime();
    if( localTime > latestTime )
    {
      latestTime = localTime;
    }
  }
  return latestTime;
}

template< unsigned int TDimension >
void
RenderMethod< TDimension >
::SetName( const char * name )
{
  m_Name = name;
}

template< unsigned int TDimension >
const char *
RenderMethod< TDimension >
::GetName() const
{
  return m_Name.c_str();
}

template< unsigned int TDimension >
const char *
RenderMethod< TDimension >
::GetType() const
{
  return m_Type.c_str();
}

} // end of namespace sov
