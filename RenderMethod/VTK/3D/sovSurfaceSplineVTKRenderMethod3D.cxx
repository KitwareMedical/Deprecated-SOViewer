/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceSplineVTKRenderMethod3D.cxx,v $
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
#include "sovSurfaceSplineVTKRenderMethod3D.h"
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkTubeFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>

#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h> 
#include <vtkFloatArray.h>
#include <vtkProperty.h>

namespace sov
{

/** Constructor */
SurfaceSplineVTKRenderMethod3D
::SurfaceSplineVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("SurfaceSpatialObject");

  m_nbPoints = 100;
  m_xGridSize = 100;
  m_yGridSize = 100;
  m_zGridSize = 100;
  m_xVar = 100;
  m_yVar = 100;
  m_zVar = 100;
  m_sigma = 5;
  m_Direction = 0;
//  m_DepthFactor = 1;
  
  m_dataSetLoaded = false;
//  m_representationMode = Surface; //Point_
//  m_interpolationMode = Phong; //Flat
  m_targetVisible = true;
  m_faceVisible = true;
//  m_3DRenderInit = true;


  m_faceMapper = vtkDataSetMapper::New();
//  m_faceActor = vtkActor::New();

  m_thinPlate = vtkThinPlateSplineTransform::New();
  m_thinPlateFilter = vtkTransformFilter::New();

  m_grid = vtkRectilinearGrid::New();
  m_gridGeometry = vtkRectilinearGridGeometryFilter::New();

  m_target = vtkUnstructuredGrid::New();
//  m_targetActor = vtkActor::New();
  m_targetMapper = vtkDataSetMapper::New();

  m_xGridCoords = NULL;
  m_yGridCoords = NULL;
  m_zGridCoords = NULL;

  m_LookupTable = NULL;

  m_targetActor = vtkActor::New(); // BE CAREEFULL !!! should create one actor per object !

}

/** Destructor */
SurfaceSplineVTKRenderMethod3D
::~SurfaceSplineVTKRenderMethod3D()
{
}

/** */
void 
SurfaceSplineVTKRenderMethod3D
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
SurfaceSplineVTKRenderMethod3D
::Update()
{
  //std::cout << "SurfaceSplineVTKRenderMethod3D::Update() " << std::endl;
  ObjectListType::iterator            currentObject,lastObject;
 
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
  
  for(; currentObject!=lastObject; currentObject++)
  {
    if((*currentObject)->IsModified())
    {
      //std::cout << "SurfaceSplineVTKRenderMethod3D::Modified ! " << std::endl;
      RemoveActor(*currentObject); // remove the actor before creating the new one.
      SurfaceType::Pointer surface = dynamic_cast<SurfaceType *>((*currentObject)->GetObject());
      SurfacePointListType m_Point3DList = surface->GetPoints();
    
      m_Spacing = surface->GetIndexToObjectTransform()->GetScaleComponent();

      if(m_Point3DList.size()>0)
      {
        SetPoints(&m_Point3DList);
      
        ComputeThinPlateSplineTransform();

        m_ThinPlatePointSet= m_thinPlateFilter->GetOutput();

        vtkFloatArray*  m_ColorScalars = vtkFloatArray::New();
        unsigned int numPts = m_thinPlateFilter->GetOutput()->GetNumberOfPoints();
        m_ColorScalars->SetNumberOfTuples(numPts);
        
        /*float R,G,B;
        m_LookupTable->SetNumberOfColors(numPts);
        m_LookupTable->Build();

        itk::Index<2> m_Index;
        
        double stepX = (m_Maximum[0]-m_Minimum[0])/m_xGridSize;
        double stepY = (m_Maximum[1]-m_Minimum[1])/m_yGridSize;

        unsigned int j=0;
        unsigned int k=0;*/
        
        for(unsigned int i=0; i<numPts; i++)
        {
          /*if((j%m_xGridSize) == 0)
          {
            k++;
            j=0;
          }
          m_Index[0] = 3*(int)(m_Minimum[0]+j*stepX);
          m_Index[1] = m_Minimum[1]+k*stepY;
          R= m_TextureMap->GetPixel(m_Index); 
          m_Index[0]++;
          G= m_TextureMap->GetPixel(m_Index); 
          m_Index[0]++;
          B= m_TextureMap->GetPixel(m_Index); 
         
          m_LookupTable->SetTableValue(i, R/255, G/255, B/255, 1);*/
          m_ColorScalars->SetTuple1(i,0);
          //j++;
        }

        //m_ThinPlatePointSet->GetPointData()->SetScalars(m_ColorScalars);


        //std::cout << "Number of points = " << m_ThinPlatePointSet->GetNumberOfPoints() << std::endl;
  

        //vtkDataSet* m_ThinPlatePointSet= m_thinPlateFilter->GetOutput();
        //m_ThinPlatePointSet->GetPointData()->SetScalars(m_ColorScalars);

        m_faceMapper->SetInput(m_ThinPlatePointSet);
        //m_faceMapper->SetScalarRange(0,numPts);
        
        
        m_faceMapper->ScalarVisibilityOff();  //do not interpret scalars as color command

        
        /*
        vtkDataSetMapper*  m_TargetMapper = vtkDataSetMapper::New();
        m_TargetMapper->SetInput(m_ThinPlatePointSet);
        */
        //vtkUnstructuredGrid * m_target = vtkUnstructuredGrid::New();
        //m_target->SetPoints(m_targetPoints);
        //m_TargetMapper->SetInput(m_target);
        m_targetActor->SetMapper(m_faceMapper);
       
        //m_targetActor->GetProperty()->SetColor(0, 1, 0);
        //m_targetActor->GetProperty()->SetOpacity(1.0);
        //m_targetActor->GetProperty()->SetRepresentationToSurface();
        //m_targetActor->GetProperty()->SetInterpolationToFlat();

        //std::cout << "Creating Actor !!!" << std::endl;
        //mapper->SetInput(output);
        //actor->SetMapper(mapper);
        //actor->GetProperty()->SetColor(1,0,0);
   

        /*std::list<vtkProp3D*>::const_iterator it = m_ActorList.begin();

        //Add the object only if it is not already in the list
        bool exist = false;
        while(it != m_ActorList.end())
        {
          //std::cout << "m_ActorList has :" << *it << std::endl; 
          if(m_targetActor == *it)
          {
            exist = true;
          }
          it++;
        }

        if(!exist)
        {*/
          //std::cout << "Adding has :" << *it << std::endl;
          (*currentObject)->AddVTKDefined(m_targetActor);
          m_ActorList.push_back(m_targetActor);
          m_ActorToBeAddedList.push_back(m_targetActor);
          (*currentObject)->UpdateMTime();
        //}
        //m_targetActor->Delete();

        m_faceMapper->Delete();
        m_ColorScalars->Delete();
      } 
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

/** Print the method */
void
SurfaceSplineVTKRenderMethod3D
::PrintSelf (std::ostream &, Indent ) const
{
}

/** Print the method */
void
SurfaceSplineVTKRenderMethod3D
::ComputeThinPlateSplineTransform(void)
{

  if(!m_dataSetLoaded) return;

  /*if(m_LookupTable)
  {
    m_LookupTable->Delete();
  }
  m_LookupTable = vtkLookupTable::New();
  m_ColorScalars = vtkScalars::New();
*/
  if(m_xGridCoords) m_xGridCoords->Delete();
  if(m_yGridCoords) m_yGridCoords->Delete();
  if(m_zGridCoords) m_zGridCoords->Delete();

  m_xGridCoords = vtkFloatArray::New();
  m_yGridCoords = vtkFloatArray::New();
  m_zGridCoords = vtkFloatArray::New();

  m_xGridStep = m_xVar / m_xGridSize;
  m_yGridStep = m_yVar / m_yGridSize;
  m_zGridStep = m_zVar / m_zGridSize;

    int i;
  for(i=0; i<m_xGridSize; i++) m_xGridCoords->InsertNextTuple1(m_xGridStep * i);
  for(i=0; i<m_yGridSize; i++) m_yGridCoords->InsertNextTuple1(m_yGridStep * i);
  for(i=0; i<m_zGridSize; i++) m_zGridCoords->InsertNextTuple1(m_zGridStep * i);

  switch(m_Direction)
  {
    case 0:
       m_grid->SetDimensions(1,m_yGridSize,m_zGridSize);
       break;
    case 1:
       m_grid->SetDimensions(m_xGridSize,1,m_zGridSize);
       break;
    case 2:
       m_grid->SetDimensions(m_xGridSize,m_yGridSize,1);
       break; 
  }
  m_grid->SetXCoordinates(m_xGridCoords);
  m_grid->SetYCoordinates(m_yGridCoords);
  m_grid->SetZCoordinates(m_zGridCoords);
    
  m_gridGeometry->SetInput(m_grid);
  m_gridGeometry->SetExtent(0,(m_xGridSize-1), 0,(m_yGridSize-1), 0,(m_zGridSize-1));

  m_thinPlate->SetSourceLandmarks(m_sourcePoints);
  m_thinPlate->SetTargetLandmarks(m_targetPoints);
  m_thinPlate->SetSigma(m_sigma);

  m_thinPlateFilter->SetTransform(m_thinPlate);
  m_thinPlateFilter->SetInput(m_gridGeometry->GetOutput());

  m_thinPlateFilter->Update();

 
  // Apply the texture
/*  unsigned int numPts = m_thinPlateFilter->GetOutput()->GetNumberOfPoints();
  m_ColorScalars->SetNumberOfScalars(numPts);
  
  float R,G,B;
  m_LookupTable->SetNumberOfColors(numPts);
  m_LookupTable->Build();

  itk::Index<2> m_Index;
  
  double stepX = (m_Maximum[0]-m_Minimum[0])/m_xGridSize;
  double stepY = (m_Maximum[1]-m_Minimum[1])/m_yGridSize;

  unsigned int j=0;
  unsigned int k=0;
  
  /*for(unsigned int i=0; i<numPts; i++)
  {
    
    if((j%m_xGridSize) == 0)
    {
      k++;
      j=0;
    }
    m_Index[0] = 3*(int)(m_Minimum[0]+j*stepX);
    m_Index[1] = m_Minimum[1]+k*stepY;
    R= m_TextureMap->GetPixel(m_Index); 
    m_Index[0]++;
    G= m_TextureMap->GetPixel(m_Index); 
    m_Index[0]++;
    B= m_TextureMap->GetPixel(m_Index); 
    
    R=255;
    G=0;
    B=0;
    m_LookupTable->SetTableValue(i, R/255, G/255, B/255, 1);
    m_ColorScalars->SetScalar(i,i);
    j++;
  }

  /*vtkDataSet* m_ThinPlatePointSet= m_thinPlateFilter->GetOutput();
  m_ThinPlatePointSet->GetPointData()->SetScalars(m_ColorScalars);
  m_faceMapper->SetInput(m_ThinPlatePointSet);
  m_ColorScalars->Delete();
  m_faceMapper->SetScalarRange(0,numPts);*/

}

/** Set the points */
void
SurfaceSplineVTKRenderMethod3D
::SetPoints(SurfacePointListType * list)
{

  if(m_dataSetLoaded)
  {
    m_xTargetCoords->Delete();
    m_yTargetCoords->Delete();
    m_zTargetCoords->Delete();

    m_xSourceCoords->Delete();
    m_ySourceCoords->Delete();
    m_zSourceCoords->Delete();

    m_targetPoints->Delete();
    m_sourcePoints->Delete();

    m_target->Delete();
    m_target = vtkUnstructuredGrid::New();
  }

  m_xTargetCoords = vtkFloatArray::New();
  m_yTargetCoords = vtkFloatArray::New();
  m_zTargetCoords = vtkFloatArray::New();

  m_xSourceCoords = vtkFloatArray::New();
  m_ySourceCoords = vtkFloatArray::New();
  m_zSourceCoords = vtkFloatArray::New();
  
  m_targetPoints = vtkPoints::New();
  m_sourcePoints = vtkPoints::New();
  
  //std::cout << "m_Point3DList.size() 2 = " << list->size() << std::endl;

  SurfacePointListType::iterator point3DListIterator = list->begin();

  for(unsigned int i=0;i<3;i++)
  {
    m_Minimum[i] = 9999; 
    m_Maximum[i] = -1;
  }
  

  while(point3DListIterator != list->end())
  {
    if((*point3DListIterator).GetPosition()[0]*m_Spacing[0] < m_Minimum[0])
    {
      m_Minimum[0]=(*point3DListIterator).GetPosition()[0]*m_Spacing[0];
    }
    if((*point3DListIterator).GetPosition()[1]*m_Spacing[1] < m_Minimum[1])
    {
      m_Minimum[1]=(*point3DListIterator).GetPosition()[1]*m_Spacing[1];
    }
    if((*point3DListIterator).GetPosition()[2]*m_Spacing[2] < m_Minimum[2])
    {
      m_Minimum[2]=(*point3DListIterator).GetPosition()[2]*m_Spacing[2];
    }
    if((*point3DListIterator).GetPosition()[0]*m_Spacing[0] > m_Maximum[0])
    {
      m_Maximum[0]=(*point3DListIterator).GetPosition()[0]*m_Spacing[0];
    }
    if((*point3DListIterator).GetPosition()[1]*m_Spacing[1] >m_Maximum[1])
    {
      m_Maximum[1]=(*point3DListIterator).GetPosition()[1]*m_Spacing[1];
    }
    if((*point3DListIterator).GetPosition()[2]*m_Spacing[2] >m_Maximum[2])
    {
      m_Maximum[2]=(*point3DListIterator).GetPosition()[2]*m_Spacing[2];
    }
    point3DListIterator++;
  }
  
  m_xVar = m_Maximum[0]-m_Minimum[0];
  m_yVar = m_Maximum[1]-m_Minimum[1];
  m_zVar = m_Maximum[2]-m_Minimum[2];

  point3DListIterator = list->begin();
  
  for(unsigned int i=0; i<list->size(); i++)
  {

    m_xTargetCoords->InsertNextTuple1(((*point3DListIterator).GetPosition()[0]*m_Spacing[0]-m_Minimum[0]+1));//*m_xVar/(m_Maximum[0]-m_Minimum[0]));
    m_yTargetCoords->InsertNextTuple1(((*point3DListIterator).GetPosition()[1]*m_Spacing[1]-m_Minimum[1]+1));//*m_yVar/(m_Maximum[1]-m_Minimum[1]));
    m_zTargetCoords->InsertNextTuple1(((*point3DListIterator).GetPosition()[2]*m_Spacing[2]-m_Minimum[2]+1));//*m_zVar/(m_Maximum[2]-m_Minimum[2]));
  
    m_xSourceCoords->InsertNextTuple1(m_xTargetCoords->GetTuple1(i));
    m_ySourceCoords->InsertNextTuple1(m_yTargetCoords->GetTuple1(i));
  m_zSourceCoords->InsertNextTuple1(m_zTargetCoords->GetTuple1(i));

    m_targetPoints->InsertPoint(i,m_xTargetCoords->GetTuple1(i),m_yTargetCoords->GetTuple1(i),m_zTargetCoords->GetTuple1(i));
    
    switch(m_Direction)
    {
      case 0:
        m_sourcePoints->InsertPoint(i,0,m_ySourceCoords->GetTuple1(i),m_zSourceCoords->GetTuple1(i));
        break;
      case 1:
        m_sourcePoints->InsertPoint(i,m_xSourceCoords->GetTuple1(i),0,m_zSourceCoords->GetTuple1(i));
        break;
      case 2:
        m_sourcePoints->InsertPoint(i,m_xSourceCoords->GetTuple1(i),m_ySourceCoords->GetTuple1(i),0);
        break; 
    }


    m_target->InsertNextCell(VTK_VERTEX,1,(vtkIdType*)&i);
  
  
    point3DListIterator++;
  }

  if(list->size()>0)
  {
    m_dataSetLoaded = true;
  }
  else
  {
    m_dataSetLoaded = false;
  }
}

} // end of namespace sov
