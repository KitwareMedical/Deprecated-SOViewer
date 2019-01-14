/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSplineTubeVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:58 $
  Version:   $Revision: 1.4 $
  Author:    Pierre Fillard (fillard@cs.unc.edu)

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovSplineTubeVTKRenderMethod3D.h"
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

#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif

namespace sov
{

/** Constructor */
SplineTubeVTKRenderMethod3D
::SplineTubeVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("VesselTubeSpatialObject");
  AddSupportedType("DTITubeSpatialObject");
  AddSupportedType("TubeSpatialObject");
  m_NumberOfSides = 5;
  m_PointCoeff = 100;
}

/** Destructor */
SplineTubeVTKRenderMethod3D
::~SplineTubeVTKRenderMethod3D()
{
}

/** */
void 
SplineTubeVTKRenderMethod3D
::draw( void )
{
}

void
SplineTubeVTKRenderMethod3D
::SetNumberOfPoints(double n)
{
  m_PointCoeff=n;
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
SplineTubeVTKRenderMethod3D
::Update()
{
  ObjectListType::iterator            currentObject,lastObject;

  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();


  for(; currentObject!=lastObject; currentObject++)
  {
    if((*currentObject)->IsModified())
    {
      bool showExtremities = static_cast<VisualTubeProperty *>((*currentObject)->GetProperty())->GetShowExtremities();
 
      // Remove all precedent actors
      std::list<void*> list = (*currentObject)->GetVTKDefinedList();
      std::list<void*>::iterator it = list.begin();
      while(it != list.end())
      {
        vtkActor* act = static_cast<vtkActor*>(*it);
        it++;
        act->Delete();
      }

      RemoveActor(*currentObject); // remove the actor before creating the new one 

      TubeType::Pointer tube = static_cast<TubeType *>((*currentObject)->GetObject());
      unsigned int i;

      if(m_NumberOfSides < 3)
      {
        m_NumberOfSides = 3;
      }
     
      if(tube->GetNumberOfPoints() < 2)
        {
        //std::cout << "Not enough points" << std::endl;
        }
      
      const double* spacing = tube->GetIndexToObjectTransform()->GetScaleComponent();

      //Pre-Step: interpolate the points into a 3D-BSpline
      MySpline3D m_spline;
      m_spline.SetInput(tube);
      m_spline.SetNbPointsInterpolated(m_PointCoeff*tube->GetNumberOfPoints());
      
      
      //std::cout << "before interp" << std::endl;
      TubePointListType TubeSplinePoint = m_spline.GetInterpolatedTubePointList();
      //std::cout << "after interp" << std::endl;


      //Step 1: copy skeleton points from a vessel into vtkPoints
      //vtkpoints assumes a triplet is coming so use pointer arithmetic
      //to jump to the next spot in a multidimensional array
      TubePointListType::iterator  pnt = TubeSplinePoint.begin();

      unsigned int nPoints = TubeSplinePoint.size();
      vtkPoints* vPoints = vtkPoints::New();
      vPoints->SetNumberOfPoints(nPoints);

      vtkFloatArray* vScalars = vtkFloatArray::New();
      vScalars->SetNumberOfTuples(nPoints);


      if(showExtremities)
      {
        vtkSphereSource * sphereSource1 = vtkSphereSource::New();
        sphereSource1->SetCenter((float)((*pnt).GetPosition()[0]*spacing[0]), (float)((*pnt).GetPosition()[1]*spacing[1]), (float)((*pnt).GetPosition()[2]*spacing[2]));
        sphereSource1->SetRadius((*pnt).GetRadius()*0.95*spacing[0]);
  
        vtkPolyDataMapper *sphereMapper1 = vtkPolyDataMapper::New();
        sphereMapper1->SetInput(sphereSource1->GetOutput());
  
        vtkActor* sphere1 = vtkActor::New();
        sphere1->SetMapper(sphereMapper1);
  
        sphere1->GetProperty()->SetColor(tube->GetProperty()->GetRed(),
                                         tube->GetProperty()->GetGreen(),
                                         tube->GetProperty()->GetBlue()); 
        sphere1->GetProperty()->SetOpacity(1.0); 
     
        m_ActorList.push_back(sphere1);
        m_ActorToBeAddedList.push_back(sphere1);
        (*currentObject)->AddVTKDefined(sphere1);
        sphereMapper1->Delete();
        sphereSource1->Delete();
      }
   

      for(i=0; pnt!=TubeSplinePoint.end(); pnt++)
      {
        vPoints->SetPoint(i, (float)((*pnt).GetPosition()[0]*spacing[0]), (float)((*pnt).GetPosition()[1]*spacing[1]), (float)((*pnt).GetPosition()[2]*spacing[2]));
        vScalars->SetTuple1(i,(*pnt).GetRadius()*0.95*spacing[0]);
        i++;
      }  

      if(showExtremities)
        {
          pnt--;
          vtkSphereSource * sphereSource2 = vtkSphereSource::New();
          sphereSource2->SetCenter((float)((*pnt).GetPosition()[0]*spacing[0]), (float)((*pnt).GetPosition()[1]*spacing[1]), (float)((*pnt).GetPosition()[2]*spacing[2]));
          sphereSource2->SetRadius((*pnt).GetRadius()*0.95*spacing[0]);
  
          vtkPolyDataMapper *sphereMapper2 = vtkPolyDataMapper::New();
          sphereMapper2->SetInput(sphereSource2->GetOutput());
  
          vtkActor* sphere2 = vtkActor::New();
          sphere2->SetMapper(sphereMapper2);
  
          sphere2->GetProperty()->SetColor(tube->GetProperty()->GetRed(),
                                           tube->GetProperty()->GetGreen(),
                                           tube->GetProperty()->GetBlue()); 
          sphere2->GetProperty()->SetOpacity(1.0); 

          (*currentObject)->AddVTKDefined(sphere2);
          m_ActorList.push_back(sphere2);
          m_ActorToBeAddedList.push_back(sphere2);

          sphereMapper2->Delete();
          sphereSource2->Delete();
        }

        //
        //Step 2: create a point id list (for a polyline this is just linear
        vtkIdType* pntIds = new vtkIdType[nPoints];
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
  
      //Step 9: create a mapper of the tube
      vtkPolyDataMapper* vMapper = vtkPolyDataMapper::New();

      vMapper->SetInput(vTFilter->GetOutput());
      
      vMapper->ScalarVisibilityOff();  //do not interpret scalars as color command

      //Step 10: Add the mapper to the actor. You can now delete everything.
      //A matrix for the actor, colors, opacities, etc can be set by
      //the caller before or after this function is called.
      
      vtkActor* actor = vtkActor::New();
      actor->SetMapper(vMapper);

      actor->GetProperty()->SetColor(tube->GetProperty()->GetRed(),
                                     tube->GetProperty()->GetGreen(),
                                     tube->GetProperty()->GetBlue()); 
      actor->GetProperty()->SetOpacity(1.0); 
      (*currentObject)->AddVTKDefined(actor);
      m_ActorList.push_back(actor);
      m_ActorToBeAddedList.push_back(actor);
      (*currentObject)->UpdateMTime();

      vPoints->Delete();
      delete [] pntIds;
      vScalars->Delete();
      vPLine->Delete();
      vClean->Delete();
      vCA->Delete();
      vPData->Delete();
      vTFilter->Delete();
      vMapper->Delete();
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
SplineTubeVTKRenderMethod3D
::PrintSelf (std::ostream &, Indent ) const
{
}

} // end of namespace sov
