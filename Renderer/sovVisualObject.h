/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVisualObject.h,v $
  Language:  C++
  Date:      $Date: 2005-12-15 19:50:15 $
  Version:   $Revision: 1.6 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __VisualObject_h
#define __VisualObject_h

#include <iostream>

#include "sovVisualObjectProperty.h"
#include "sovVisualImageProperty.h"
#include "sovVisualTubeProperty.h"
#include "sovVisualMeshProperty.h"
#include <itkSpatialObject.h>


namespace sov
{


template< unsigned int TDimension >
class VisualObject
{

public:

  typedef itk::SpatialObject<TDimension>   ObjectType;
  typedef typename ObjectType::Pointer     ObjectPointer;

  /** This list is used by VTK Render Methods to store VTK Actors pointers.
   *  this list should not be used for other purposes */
  typedef std::list<void*> VTKDefinedListType;

  void SetObject(ObjectType* object);
  ObjectType* GetObject();

  typedef VisualObjectProperty             PropertyType;

  PropertyType* GetProperty(void) {return m_Property;}
  void SetProperty(PropertyType* prop) {m_Property = prop;}
  unsigned long GetMTime(void) {return m_MTime;}
  void SetMTime(unsigned long time) {m_MTime = time;}
  void UpdateMTime(void);
  void UpdateTransformMTime(void);
  bool IsModified(void);
  bool IsObjectPropertyModified(void);
  bool IsPropertyModified(void);
  bool IsTransformModified(void);
  void UpdateObjectPropertyMTime(void);
  void UpdatePropertyMTime(void);
  void UpdatePropertyAskingForUpdateMTime(void);
  bool IsPropertyAskingForUpdate(void);

  /** Increment the modified time of the object 
   *  This forces an update on the object */
  void Modified() {m_MTime--;}

  VisualObject();
  virtual ~VisualObject();


  /** Manage the user defined list */
  void AddVTKDefined(void* pointer);
  bool RemoveVTKDefined(void* pointer);
  VTKDefinedListType GetVTKDefinedList(void);
   
  void SetRenderMethodName(std::string name);
  const char * GetRenderMethodName(void) const;
   
  /** Set/Get the render method associated with the object */
  NDimensionalRenderMethod* GetRenderMethod(void) const;
  void SetRenderMethod(NDimensionalRenderMethod* method);

protected:

  ObjectPointer m_Object;
  PropertyType* m_Property;
  unsigned long m_MTime;
  unsigned long m_TransformMTime ;
  unsigned long m_GlobalTransformMTime ;
  unsigned long m_ObjectPropertyMTime;
  unsigned long m_PropertyMTime;
  unsigned long m_PropertyAskRenderMethodToBeUpdatedTime;

  VTKDefinedListType        m_VTKDefinedList;
  std::string                m_RenderMethodName;
  NDimensionalRenderMethod * m_RenderMethod;
};

} //end of namespace sov


#ifndef ITK_MANUAL_INSTANTIATION
  #include "sovVisualObject.txx"
#endif


#endif //__sovVisualObject_h
