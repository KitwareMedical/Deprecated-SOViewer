/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovCenterLineTubeVTKRenderMethod3D.cxx,v $
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
#include "sovCenterLineTubeVTKRenderMethod3D.h"
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
#include <vtkProperty.h>

#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif

namespace sov
{

/** Constructor */
CenterLineTubeVTKRenderMethod3D
::CenterLineTubeVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("VesselTubeSpatialObject");
  AddSupportedType("DTITubeSpatialObject");
  AddSupportedType("TubeSpatialObject");
}

/** Destructor */
CenterLineTubeVTKRenderMethod3D
::~CenterLineTubeVTKRenderMethod3D()
{
}

/** */
void 
CenterLineTubeVTKRenderMethod3D
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
CenterLineTubeVTKRenderMethod3D
::Update()
{

  ObjectListType::iterator            currentObject,lastObject;

  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
  {
    if((*currentObject)->IsModified())
    {
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

      const double* spacing = tube->GetIndexToObjectTransform()->GetScaleComponent();

      //Step 1: copy skeleton points from a vessel into vtkPoints
      //vtkpoints assumes a triplet is coming so use pointer arithmetic
      //to jump to the next spot in a multidimensional array
      unsigned int nPoints = tube->GetNumberOfPoints();
      vtkPoints* vPoints = vtkPoints::New();
      vPoints->SetNumberOfPoints(nPoints);

      vtkFloatArray* vScalars = vtkFloatArray::New();
      vScalars->SetNumberOfTuples(nPoints);

  
      for(i=0;i<tube->GetNumberOfPoints();i++)
        {
        const TubeType::TubePointType* pt = static_cast<const TubeType::TubePointType*>(tube->GetPoint(i));
        vPoints->SetPoint(i, (float)(pt->GetPosition()[0]*spacing[0]), (float)(pt->GetPosition()[1]*spacing[1]), (float)(pt->GetPosition()[2]*spacing[2]));
        vScalars->SetTuple1(i,pt->GetRadius()*spacing[0]*0.95);
        }
  

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
        vPData->GetPointData()->SetScalars(vScalars);
      }
        
      //Step 7: remove any duplicate points from polydata. The tube filter
      //fails if any duplicates are present
      vtkCleanPolyData* vClean = vtkCleanPolyData::New();
      vClean->SetInput(vPData);

      //Step 9: create a mapper of the tube
      vtkPolyDataMapper* vMapper = vtkPolyDataMapper::New();
      vMapper->SetInput(vClean->GetOutput());   
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
      vCA->Delete();
      vClean->Delete();
      vPData->Delete();
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
CenterLineTubeVTKRenderMethod3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}

} // end of namespace sov
