/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovTubePhantomRenderMethod.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:56 $
  Version:   $Revision: 1.4 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovTubePhantomRenderMethod.h"
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkTubeFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>

#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h> 
#include <vtkFloatArray.h>
#include <sovVisualTubeProperty.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkTriangleFilter.h>

#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif

namespace sov
{

/** Constructor */
TubePhantomRenderMethod
::TubePhantomRenderMethod()
{
  m_SupportedTypeList->clear();
  AddSupportedType("TubeSpatialObject");
  m_NumberOfSides = 3;
}

/** Destructor */
TubePhantomRenderMethod
::~TubePhantomRenderMethod()
{
}

/** */
void 
TubePhantomRenderMethod
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
TubePhantomRenderMethod
::Update()
{
  ObjectListType::iterator            currentObject,lastObject;

  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  unsigned int tubeIndex=0;
  unsigned int pointIndex=0;

  for(; currentObject!=lastObject; currentObject++)
  {
    if((*currentObject)->IsModified())
    { 
      bool showExtremities = false; //static_cast<VisualTubeProperty *>((*currentObject)->GetProperty())->GetShowExtremities();
  
      // Remove all precedent actors
      /*std::list<void*> list = (*currentObject)->GetVTKDefinedList();
      std::list<void*>::iterator it = list.begin();
      while(it != list.end())
      {
        vtkActor* act = static_cast<vtkActor*>(*it);
        it++;
        act->Delete();
      }*/

      //RemoveActor(*currentObject); // remove the actor before creating the new one 
      
      TubeType::Pointer tube = dynamic_cast<TubeType *>((*currentObject)->GetObject());
      unsigned int i;

      if(m_NumberOfSides < 3)
      {
        m_NumberOfSides = 3;
      }
     
      if(tube->GetPoints().size() < 2)
      {
        //std::cout << "Not enough points" << std::endl;
      }
      else
      {
        const double* spacing = tube->GetIndexToObjectTransform()->GetScaleComponent();

        //Step 1: copy skeleton points from a vessel into vtkPoints
        //vtkpoints assumes a triplet is coming so use pointer arithmetic
        //to jump to the next spot in a multidimensional array
        TubePointListType::iterator  pnt = tube->GetPoints().begin();
        unsigned int nPoints = tube->GetPoints().size();
        vtkPoints* vPoints = vtkPoints::New();
        vPoints->SetNumberOfPoints(nPoints);

        vtkFloatArray* vScalars = vtkFloatArray::New();
        vScalars->SetNumberOfTuples(nPoints);

        if(showExtremities)
          {
          vtkSphereSource * sphereSource1 = vtkSphereSource::New();
          sphereSource1->SetCenter((float)((*pnt).GetPosition()[0]*spacing[0]), (float)((*pnt).GetPosition()[1]*spacing[1]), (float)((*pnt).GetPosition()[2]*spacing[2]));
          sphereSource1->SetRadius((*pnt).GetRadius()*0.95);
  
          vtkTriangleFilter* triangleFilter = vtkTriangleFilter::New();
          triangleFilter->SetInput(sphereSource1->GetOutput());
          triangleFilter->Update();
      
          int NumVerts = triangleFilter->GetOutput()->GetNumberOfPoints();
          int NumTris = triangleFilter->GetOutput()->GetNumberOfCells();   
          
          // Create the list of vertices
          double* verts;
          verts = new double[NumVerts*3];
          unsigned long j = 0;
          for(unsigned int i=0;i<NumVerts;i++)
            {
            verts[j++] = triangleFilter->GetOutput()->GetPoint(i)[0];
            verts[j++] = triangleFilter->GetOutput()->GetPoint(i)[1];
            verts[j++] = triangleFilter->GetOutput()->GetPoint(i)[2];
            }
            
          int* tris;
          tris = new int[NumTris*3];
          j=0;
          for(unsigned int i=0;i<NumTris;i++)
            {
            tris[j++] = triangleFilter->GetOutput()->GetCell(i)->GetPointId(0);
            tris[j++] = triangleFilter->GetOutput()->GetCell(i)->GetPointId(1);
            tris[j++] = triangleFilter->GetOutput()->GetCell(i)->GetPointId(2);
            }
          
          gwpTriPolyMeshHaptic* triMeshHaptic =  
          new gwpTriPolyMeshHaptic(NumVerts,3,verts, 
                                  NumTris,3,tris);
                                   
          triMeshHaptic->setTouchableFrom(gwpTriPolyMeshHaptic::RV_FRONT);

        
          (*currentObject)->AddVTKDefined(triMeshHaptic);
          m_ActorList.push_back(triMeshHaptic);
          m_ActorToBeAddedList.push_back(triMeshHaptic);

          sphereSource1->Delete();
          }
  
        for(i=0; pnt!=tube->GetPoints().end(); pnt++)
        {
          vPoints->SetPoint(i, (float)((*pnt).GetPosition()[0]*spacing[0]), (float)((*pnt).GetPosition()[1]*spacing[1]), (float)((*pnt).GetPosition()[2]*spacing[2]));
          vScalars->SetTuple1(i,(*pnt).GetRadius());
          i++; 
        }  
  
        
        if(showExtremities)
        {
          pnt--;
          vtkSphereSource * sphereSource2 = vtkSphereSource::New();
          sphereSource2->SetCenter((float)((*pnt).GetPosition()[0]*spacing[0]), (float)((*pnt).GetPosition()[1]*spacing[1]), (float)((*pnt).GetPosition()[2]*spacing[2]));
          sphereSource2->SetRadius((*pnt).GetRadius()*0.95);
          
          vtkTriangleFilter* triangleFilter = vtkTriangleFilter::New();
          triangleFilter->SetInput(sphereSource2->GetOutput());
          triangleFilter->Update();
      
          int NumVerts = triangleFilter->GetOutput()->GetNumberOfPoints();
          int NumTris = triangleFilter->GetOutput()->GetNumberOfCells();   
          
          // Create the list of vertices
          double* verts;
          verts = new double[NumVerts*3];
          unsigned long j = 0;
          for(unsigned int i=0;i<NumVerts;i++)
            {
            verts[j++] = triangleFilter->GetOutput()->GetPoint(i)[0];
            verts[j++] = triangleFilter->GetOutput()->GetPoint(i)[1];
            verts[j++] = triangleFilter->GetOutput()->GetPoint(i)[2];
            }
            
          int* tris;
          tris = new int[NumTris*3];
          j=0;
          for(unsigned int i=0;i<NumTris;i++)
            {
            tris[j++] = triangleFilter->GetOutput()->GetCell(i)->GetPointId(0);
            tris[j++] = triangleFilter->GetOutput()->GetCell(i)->GetPointId(1);
            tris[j++] = triangleFilter->GetOutput()->GetCell(i)->GetPointId(2);
            }
          
          gwpTriPolyMeshHaptic* triMeshHaptic =  
          new gwpTriPolyMeshHaptic(NumVerts,3,verts, 
                                  NumTris,3,tris);
                                   
          triMeshHaptic->setTouchableFrom(gwpTriPolyMeshHaptic::RV_FRONT);

        
          (*currentObject)->AddVTKDefined(triMeshHaptic);
          m_ActorList.push_back(triMeshHaptic);
          m_ActorToBeAddedList.push_back(triMeshHaptic);
          sphereSource2->Delete();
        }


        //Step 2: create a point id list (for a polyline this is just linear
        int* pntIds = new int[nPoints];
        for (i = 0; i < (int)nPoints; i++)
        {
          pntIds[i] = i;
        }

        //Step3: create a polyline from the points and pt id list
        vtkPolyLine* vPLine = vtkPolyLine::New();
        vPLine->Initialize(nPoints, pntIds, vPoints);

        //Step 4: convert this to a cellarray (required for input to polydata)
        vtkCellArray* vCA = vtkCellArray::New();
        vCA->InsertNextCell(vPLine); 

        //Step 5: create a scalar array that indicates the radius at each
        //skeleton point. Vtk's way of setting radius is screwy: it fails if every
        //point has the same radius. It also uses a minimum radius  (called radius)
        //and a max radius (defined by scale factor). In order to get this to work, 
        //you need to find the min and max of your vessel radii--if the same, later
        //set a constant radius in the tube filter. If not the same, you need to 
        //define the min radius and the ratio max/min. If you send these params,
        //the tube will end up with proper radius settings. Weird.

        //Step 6: convert to polydata (required for input to anything else)
        vtkPolyData* vPData = vtkPolyData::New();
        vPData->SetLines(vCA);
        vPData->SetPoints(vPoints);
        vtkFloatingPointType range[2];
        bool use_scalars = false;
        float min_scalar, max_scalar;
        vScalars->GetRange(range);
        min_scalar = range[0];
        max_scalar = range[1];
        if (min_scalar <= 0.0001) 
        {
          min_scalar = 0.0001;
        }
        if(max_scalar < min_scalar) 
        {
          max_scalar = min_scalar;
        }
        if (min_scalar != max_scalar) 
        {
          use_scalars = true;
          vPData->GetPointData()->SetScalars(vScalars);
        }
        
        //Step 7: remove any duplicate points from polydata. The tube filter
        //fails if any duplicates are present
        vtkCleanPolyData* vClean = vtkCleanPolyData::New();
        vClean->SetInput(vPData);

        //Step 8: make tubes. The number of sides per tube is set by nsides.
        //Even an nsides of 3 looks surprisingly good.
        vtkTubeFilter* vTFilter = vtkTubeFilter::New();
        vTFilter->SetNumberOfSides(m_NumberOfSides);
        vTFilter->SetInput(vClean->GetOutput());
        vTFilter->CappingOff();

        if(use_scalars)
        {
          vTFilter->SetVaryRadiusToVaryRadiusByScalar();
          vTFilter->SetRadius(min_scalar);   //this call sets min rad. Weird.
          vTFilter->SetRadiusFactor(max_scalar/min_scalar); //sets max rad. Weird
        }
        else
        {
          vTFilter->SetRadius(min_scalar);
          vTFilter->SetVaryRadiusToVaryRadiusOff();
        }

      vtkTriangleFilter* triangleFilter = vtkTriangleFilter::New();
      triangleFilter->SetInput(vTFilter->GetOutput());
      triangleFilter->Update();
      
      int NumVerts = triangleFilter->GetOutput()->GetNumberOfPoints();
      int NumTris = triangleFilter->GetOutput()->GetNumberOfCells();
            
      // Create the list of vertices
      double* verts;
      verts = new double[NumVerts*3];
      unsigned long j = 0;
      for(unsigned int i=0;i<NumVerts;i++)
        {
        verts[j++] = triangleFilter->GetOutput()->GetPoint(i)[0];
        verts[j++] = triangleFilter->GetOutput()->GetPoint(i)[1];
        verts[j++] = triangleFilter->GetOutput()->GetPoint(i)[2];
        }
        
      int* tris;
      tris = new int[NumTris*3];
      j=0;
      for(unsigned int i=0;i<NumTris;i++)
        {
        tris[j++] = triangleFilter->GetOutput()->GetCell(i)->GetPointId(0);
        tris[j++] = triangleFilter->GetOutput()->GetCell(i)->GetPointId(1);
        tris[j++] = triangleFilter->GetOutput()->GetCell(i)->GetPointId(2);
        }
      
      gwpTriPolyMeshHaptic* triMeshHaptic =  
      new gwpTriPolyMeshHaptic(NumVerts,3,verts, 
                               NumTris,3,tris);
                               
      triMeshHaptic->setTouchableFrom(gwpTriPolyMeshHaptic::RV_FRONT);

      
        (*currentObject)->AddVTKDefined(triMeshHaptic);
        m_ActorList.push_back(triMeshHaptic);
        m_ActorToBeAddedList.push_back(triMeshHaptic);
        (*currentObject)->UpdateMTime();

        vPoints->Delete();
        delete [] pntIds;
        vScalars->Delete();
        vPLine->Delete();
        vCA->Delete();
        vClean->Delete();
        vPData->Delete();
        vTFilter->Delete();
        triangleFilter->Delete();
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
TubePhantomRenderMethod
::PrintSelf (std::ostream &os, Indent indent) const
{
}

} // end of namespace sov
