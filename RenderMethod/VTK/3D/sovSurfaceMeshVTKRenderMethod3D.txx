/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceMeshVTKRenderMethod3D.txx,v $
  Language:  C++
  Date:      $Date: 2005-12-15 19:48:52 $
  Version:   $Revision: 1.13 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "sovSurfaceMeshVTKRenderMethod3D.h"

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>
#include <vtkPointSource.h>
#include <vtkSphereSource.h>
#include <vtkCellType.h>
#include <vtkPoints.h>
#include <itkCellInterface.h>

namespace sov
{

template <class TMesh>
SurfaceMeshVTKRenderMethod3D<TMesh>
::SurfaceMeshVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("MeshSpatialObject");
  this->m_PixelType = typeid(PixelType).name();
}

template <class TMesh>
SurfaceMeshVTKRenderMethod3D<TMesh>
::~SurfaceMeshVTKRenderMethod3D( void )
{
}

template <class TMesh>
void
SurfaceMeshVTKRenderMethod3D<TMesh>
::Update()
{
  ObjectListType::const_iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
  { 

  if((*currentObject)->IsModified())
    {
    vtkPolyData* polyData = vtkPolyData::New();
    vtkPoints* polyPoints = vtkPoints::New();

    RemoveActor(*currentObject); // remove the actor before creating the new one.
    SurfaceMeshType* SurfaceMeshSO = dynamic_cast<SurfaceMeshType *>((*currentObject)->GetObject());

    MeshType* mesh = SurfaceMeshSO->GetMesh();
    typename MeshType::PointsContainerPointer points = mesh->GetPoints();
    typename MeshType::PointsContainer::const_iterator it = points->begin();
    typedef typename MeshType::CellType CellType;

    unsigned long nPoints = points->size();
    polyPoints->SetNumberOfPoints(nPoints);

    for(;it != points->end();it++)
      {
      polyPoints->SetPoint((*it).first,(*it).second[0],(*it).second[1],(*it).second[2]);
      }    

    typename MeshType::CellsContainer* cells = mesh->GetCells();
    typename MeshType::CellsContainer::ConstIterator it_cells = cells->Begin();
    unsigned long nCells = cells->size();
    
    polyData->Allocate(nCells);
    
    for(;it_cells != cells->End();it_cells++)
      {
      vtkIdList* pts = vtkIdList::New();     
      typename MeshType::CellTraits::PointIdConstIterator itptids = (*it_cells)->Value()->GetPointIds();
      unsigned int id =0;
      unsigned long ptsSize = (*it_cells)->Value()->GetNumberOfPoints();
      pts->SetNumberOfIds(ptsSize);
      while(itptids != (*it_cells)->Value()->PointIdsEnd())
        {
        pts->SetId(id,*itptids);
        itptids++;
        id++;
        }

      switch((*it_cells)->Value()->GetType())
        {
        case CellType::VERTEX_CELL: polyData->InsertNextCell(VTK_VERTEX,pts);
        break;
        case CellType::LINE_CELL: polyData->InsertNextCell(VTK_LINE,pts);
        break;
        case CellType::TRIANGLE_CELL: polyData->InsertNextCell(VTK_TRIANGLE,pts);
        break;
        case CellType::TETRAHEDRON_CELL: polyData->InsertNextCell(VTK_POLYGON,pts);
        break;
        case CellType::QUADRILATERAL_CELL: polyData->InsertNextCell(VTK_QUAD,pts);
        break;
        default:
        std::cout << "SurfaceMeshVTKRenderMethod3D: Don't know how to represent cells of size " << ptsSize << std::endl;      
        }
      pts->Delete();
      }
  
    vtkPolyDataMapper *pointMapper = vtkPolyDataMapper::New();
    vtkActor* meshActor = vtkActor::New();

    meshActor->GetProperty()->SetColor(SurfaceMeshSO->GetProperty()->GetRed(),
                                       SurfaceMeshSO->GetProperty()->GetGreen(),
                                       SurfaceMeshSO->GetProperty()->GetBlue()); 

    meshActor->GetProperty()->SetOpacity(SurfaceMeshSO->GetProperty()->GetAlpha());

    polyData->SetPoints(polyPoints);
    pointMapper->SetInput(polyData);
    meshActor->SetMapper(pointMapper);

    polyPoints->Delete();
    polyData->Delete();
    pointMapper->Delete();

    (*currentObject)->AddVTKDefined(meshActor);
    m_ActorToBeAddedList.push_back(meshActor);
    m_ActorList.push_back(meshActor);
    (*currentObject)->UpdateMTime();
    }

  if((*currentObject)->IsPropertyModified())
    {
      ChangeProperty(* currentObject);
      (*currentObject)->UpdateObjectPropertyMTime();
    }

  if((*currentObject)->IsObjectPropertyModified())
    {
      ChangeProperty(* currentObject);
      (*currentObject)->UpdateObjectPropertyMTime();
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
void SurfaceMeshVTKRenderMethod3D<TMesh>
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}


} // end of namespace sov
