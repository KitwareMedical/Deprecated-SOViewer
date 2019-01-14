/*=========================================================================

  Program:   SOViewer
  Module:    $RCSfile: sovAttributeMeshVTKRenderMethod3D.txx,v $
  Language:  C++
  Date:      $Date: 2005-12-16 20:51:10 $
  Version:   $Revision: 1.5 $
  Author:    Ipek Oguz

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovAttributeMeshVTKRenderMethod3D.h"
#include <vtkArrowSourceScaled.h>

namespace sov 
{
template < class TMesh >
AttributeMeshVTKRenderMethod3D<TMesh>
::AttributeMeshVTKRenderMethod3D()
{
  this->m_SupportedTypeList->clear();
  this->AddSupportedType("MeshSpatialObject");
  this->AddSupportedType("AttributeMeshSpatialObject");
  this->m_PixelType = typeid(PixelType).name();
}

template < class TMesh >
AttributeMeshVTKRenderMethod3D<TMesh>
::~AttributeMeshVTKRenderMethod3D()
{
}

template < class TMesh >
void
AttributeMeshVTKRenderMethod3D<TMesh>
::Update()
{
  typename ObjectListType::const_iterator  currentObject,lastObject;
  currentObject = this->m_ObjectList.begin();
  lastObject = this->m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
    { 
    if((*currentObject)->IsModified())
      {
      vtkPolyData* polyData = vtkPolyData::New();
      vtkPoints* polyPoints = vtkPoints::New();
      vtkFloatArray *scalars = vtkFloatArray::New() ;
      bool scalarsValid = false ;

      this->RemoveActor(*currentObject); // remove the actor before creating the new one.
      SurfaceMeshType* SurfaceMeshSO = dynamic_cast<SurfaceMeshType *>((*currentObject)->GetObject());
              
      MeshType* mesh = SurfaceMeshSO->GetMesh();
      typename MeshType::PointsContainerPointer points = mesh->GetPoints();
      typename MeshType::PointsContainer::const_iterator it = points->begin();

      unsigned long nPoints = points->size();
      polyPoints->SetNumberOfPoints(nPoints);

      for(int i=0;it != points->end();it++, i++)
        {
        polyPoints->SetPoint((*it).first,(*it).second[0],(*it).second[1],(*it).second[2]);
        }


      typedef typename MeshType::PointDataContainer PointDataContainer;
      const PointDataContainer* pdc = mesh->GetPointData();
      if(pdc)
        {
        typename PointDataContainer::ConstIterator it_pd = pdc->Begin();
        while(it_pd != pdc->End())
          {
          if((*it_pd)->Value().Size() > 2)
            {
            it_pd++;
            continue;
            }
          scalarsValid = true;
          scalars->InsertTuple1 ((*it_pd)->Index(),(*it_pd)->Value()[0]) ;
          it_pd++;
          }
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

        switch(ptsSize)
          {
          case 3: polyData->InsertNextCell(VTK_TRIANGLE,pts);
            break;
          case 4: polyData->InsertNextCell(VTK_TETRA,pts);
            break;
          default:
            std::cout << "SurfaceMeshVTKRenderMethod3D: Don't know how to represent cells of size " << ptsSize << std::endl;      
          }
          pts->Delete();
        }
  
      vtkPolyDataMapper *pointMapper = vtkPolyDataMapper::New();
      vtkActor* meshActor = vtkActor::New();

      polyData->SetPoints(polyPoints);
      vtkPointData *pd ;
      pd = polyData->GetPointData () ;
   
      if(scalarsValid)
        {
        pd->SetScalars ( scalars );
        }
    
      if(nCells>10)
        {
        vtkLoopSubdivisionFilter *lsf = vtkLoopSubdivisionFilter::New();
        lsf->SetInput(polyData);
        lsf->SetNumberOfSubdivisions(2);
        pointMapper->SetInput(lsf->GetOutput());
        }
      else
        {
        pointMapper->SetInput(polyData);
        }        

      class sov::VisualMeshProperty *visProp ;
      class sov::ColorScheme *cs ;
      visProp = static_cast<sov::VisualMeshProperty *>((*currentObject)->GetProperty()) ;
      if (visProp)
        {
        cs = visProp->GetColorScheme();
        cs->Update(pointMapper);
        }

      meshActor->SetMapper(pointMapper);
    
      scalars->Delete() ;
      polyPoints->Delete();
      polyData->Delete();
      pointMapper->Delete();

      (*currentObject)->AddVTKDefined(meshActor);

      const double* spacing = SurfaceMeshSO->GetIndexToObjectTransform()->GetScaleComponent();
  
     if(pdc)
        {
        typename PointDataContainer::ConstIterator it_pd = pdc->Begin();
        while(it_pd != pdc->End())
          {

          if((*it_pd)->Value().Size() < 3)
            {
            it_pd++;
            continue;
            }

          // define an arrow to represent the displacement
          vtkPolyDataMapper *ArrowMapper = vtkPolyDataMapper::New();    
          vtkActor* Arrow = vtkActor::New();
          vtkArrowSourceScaled* ArrowSource = vtkArrowSourceScaled::New();

          itk::Vector<float,3> v;
          v[0] = (*it_pd)->Value()[0];
          v[1] = (*it_pd)->Value()[1];
          v[2] = (*it_pd)->Value()[2];

          Arrow->GetProperty()->SetColor(1.0,0,0);
          ArrowSource->SetLenght(v.GetNorm());
          
          typename MeshType::PointType pt;
          mesh->GetPoint((*it_pd)->Index(),&pt);


          Arrow->SetPosition(pt[0]*spacing[0],pt[1]*spacing[1], pt[2]*spacing[2]);
          
          double angley;
          double anglez = 0;
    
          #ifndef PI    
           const double PI = 4.0 * atan( 1.0 );
          #endif

          if(v[0] == 0.0)
            {
            if(v[1]>0.0)
              {
              anglez=PI/2;
              }
            else if(v[1]<0.0)
              {
              anglez=-PI/2;
              }
            }
          else
            {
            if(v[0]<0.0)
              {
              anglez = PI+atan(v[1]/v[0]);
              }
            else
              {
              anglez = atan(v[1]/v[0]);
              }
            }
          angley = -asin(v[2]);
          Arrow->SetOrientation(0.0,-angley*180/PI,-anglez*180/PI);
          
          Arrow->GetProperty()->SetOpacity(1.0); 
          ArrowMapper->SetInput(ArrowSource->GetOutput());
          Arrow->SetMapper(ArrowMapper);
          ArrowSource->Delete();
        
          (*currentObject)->AddVTKDefined(Arrow);
          this->m_ActorToBeAddedList.push_back(Arrow);
          this->m_ActorList.push_back(Arrow);
          ArrowMapper->Delete();
          it_pd++;
          }
        }
     
     



      this->m_ActorToBeAddedList.push_back(meshActor);
      this->m_ActorList.push_back(meshActor);
      (*currentObject)->UpdateMTime();
      }

    if((*currentObject)->IsPropertyModified())
      {
      this->ChangeProperty(* currentObject);
      (*currentObject)->UpdateObjectPropertyMTime();
      }

    if((*currentObject)->IsObjectPropertyModified())
      {
      this->ChangeProperty(* currentObject);
      (*currentObject)->UpdateObjectPropertyMTime();
      }

    if((*currentObject)->IsTransformModified())
      {
      this->ApplyTransform(* currentObject);
      (*currentObject)->UpdateTransformMTime();
      }
    }
  }

}
