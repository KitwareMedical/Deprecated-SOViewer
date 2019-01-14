/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovVTKRenderMethodSlicer.h,v $
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
#ifndef __VTKRenderMethodSlicer_h
#define __VTKRenderMethodSlicer_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "sovRenderMethod.h"
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPlaneSource.h>
#include <itkEuler3DTransform.h>
#include <sovVTKWin32Header.h>

namespace sov
{

class SOVVTKRenderMethod_EXPORT VTKRenderMethodSlicer
: public RenderMethod<3>
{

public:

  typedef VTKRenderMethodSlicer             Self;
  typedef RenderMethod<3>               Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;
  typedef itk::Euler3DTransform<double> TransformType; 
  
  itkNewMacro( VTKRenderMethodSlicer );

  itkTypeMacro( VTKRenderMethodSlicer, RenderMethodSlicer );

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

  void SetOrientation(unsigned int orientation)
    {
    m_Orientation = orientation;
    this->Modified();
    }

  void SetDepth(double depth) {m_Depth = depth;}
  void SetPlane(vtkPlaneSource* plane) {m_Plane = plane;}

  void SetTransform(TransformType* transform) {m_Transform = transform;}

protected:

  VTKRenderMethodSlicer();
  ~VTKRenderMethodSlicer();
 
  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();
  virtual bool ActorIsCreated(vtkActor* actor);

  std::list<vtkProp3D*> m_ActorList;
  std::list<vtkPolyData *> m_PolyDataList;
  std::list<vtkProp3D*> m_ActorToBeAddedList;
  std::list<vtkProp3D*> m_ActorToBeRemovedList;
  unsigned int          m_Orientation;
  double m_Depth;
  vtkPlaneSource*       m_Plane;
  TransformType::Pointer m_Transform;
};

}
#endif //__VTKRenderMethodSlicer_h
