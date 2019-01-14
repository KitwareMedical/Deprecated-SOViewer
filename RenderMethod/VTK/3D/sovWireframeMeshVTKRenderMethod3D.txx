/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovWireframeMeshVTKRenderMethod3D.txx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:58 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovWireframeMeshVTKRenderMethod3D.h"

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>
#include <vtkLineSource.h>

namespace sov
{

template <class TMesh>
WireframeMeshVTKRenderMethod3D<TMesh>
::WireframeMeshVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("MeshSpatialObject");
  this->m_PixelType = typeid(typename TMesh::PixelType).name();
}

template <class TMesh>
WireframeMeshVTKRenderMethod3D<TMesh>
::~WireframeMeshVTKRenderMethod3D( void )
{
}



template <class TMesh>
bool
WireframeMeshVTKRenderMethod3D<TMesh>
::IsInLineList(pair * l)
{
  typename std::list<pair>::const_iterator it = m_LineList.begin();

  for(;it != m_LineList.end();it++)
    {
    if(
      (*it).x[0] == l->x[0]
      && (*it).x[1] == l->x[1]
      && (*it).x[2] == l->x[2]
      && (*it).y[0] == l->y[0]
      && (*it).y[1] == l->y[1]
      && (*it).y[2] == l->y[2]
      )
      {
      return true;
      }
    }

  return false;
}

template <class TMesh>
void
WireframeMeshVTKRenderMethod3D<TMesh>
::Update()
{
  ObjectListType::const_iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
  { 
  if((*currentObject)->IsModified())
    {
    RemoveActor(*currentObject); // remove the actor before creating the new one.
    WireframeMeshType* WireframeMeshSO = dynamic_cast<WireframeMeshType *>((*currentObject)->GetObject());
   
    MeshType* mesh = WireframeMeshSO->GetMesh();

    typename MeshType::CellsContainerPointer cells = mesh->GetCells();
    if(!cells)
      {
      return;
      }
    typename MeshType::CellsContainer::const_iterator it = cells->begin();

    for(;it != cells->end();it++)
      {    
      typename MeshType::CellType::PointIdConstIterator it1;
      typename MeshType::CellType::PointIdConstIterator it2;

      for(it1 = (*it).second->PointIdsBegin();it1 != (*it).second->PointIdsEnd();it1++)
        {
        for(it2 = (*it).second->PointIdsBegin();it2 != (*it).second->PointIdsEnd();it2++)
          {
          if(it1 != it2)
            {
         
            pair l;
            typename MeshType::PointType pt1;
            mesh->GetPoint(*it1,&pt1);
            l.x[0]=pt1[0];
            l.x[1]=pt1[1];
            l.x[2]=pt1[2];
            typename MeshType::PointType pt2;
            mesh->GetPoint(*it2,&pt2);
            l.y[0]=pt2[0];
            l.y[1]=pt2[1];
            l.y[2]=pt2[2];

            if(!this->IsInLineList(&l))
              {
              vtkLineSource* lineSource = vtkLineSource::New(); 
              lineSource->SetPoint1(l.x);
              lineSource->SetPoint2(l.y);
              m_LineList.push_back(l);
              
              vtkPolyDataMapper *lineMapper = vtkPolyDataMapper::New();
              vtkActor* line = vtkActor::New();
              lineMapper->SetInput(lineSource->GetOutput());
              line->SetMapper(lineMapper);
              
              lineSource->Delete();
              lineMapper->Delete(); 
              (*currentObject)->AddVTKDefined(line);
              m_ActorToBeAddedList.push_back(line);
              m_ActorList.push_back(line);
              }
            }
          } 
        }
        (*currentObject)->UpdateMTime();
      }
    }

    if((*currentObject)->IsTransformModified())
    {
      ApplyTransform(* currentObject);
      (*currentObject)->UpdateTransformMTime();
    }
  }
}

/** PrintSelf */
template <class TMesh>
void WireframeMeshVTKRenderMethod3D<TMesh>
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}


} // end of namespace sov
