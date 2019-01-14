/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRenderMethod3D.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __VTKRenderMethod3D_h
#define __VTKRenderMethod3D_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "sovRenderMethod.h"
#include "vtkActor.h"
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT VTKRenderMethod3D
: public RenderMethod<3>
{

public:

  typedef VTKRenderMethod3D             Self;
  typedef RenderMethod<3>               Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;
  
  itkNewMacro( VTKRenderMethod3D );

  itkTypeMacro( VTKRenderMethod3D, RenderMethod3D );

  virtual void draw();
  
  bool IsSupported( const char* object ) const;

  std::list<vtkProp3D*> & GetActorList(void) {return m_ActorList;}
  std::list<vtkProp3D*> & GetActorToBeRemovedList(void) {return m_ActorToBeRemovedList;}
  std::list<vtkProp3D*> & GetActorToBeAddedList(void) {return m_ActorToBeAddedList;}

  void RemoveVisualObject( VisualObjectType * object );
  void RemoveActor(VisualObjectType* actor);

  void ApplyTransform(VisualObjectType * object);
  void ChangeProperty(VisualObjectType* object);
  void ChangeRepresentationType(VisualObjectType* object);

  void ClearActorToBeRemovedList(void);
  void ClearActorToBeAddedList(void){m_ActorToBeAddedList.clear();}

protected:

  VTKRenderMethod3D();
  ~VTKRenderMethod3D();
 
  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();
  virtual bool ActorIsCreated(vtkActor* actor);

  std::list<vtkProp3D*> m_ActorList;
  std::list<vtkProp3D*> m_ActorToBeAddedList;
  std::list<vtkProp3D*> m_ActorToBeRemovedList;
};

}
#endif //__VTKRenderMethod3D_h
