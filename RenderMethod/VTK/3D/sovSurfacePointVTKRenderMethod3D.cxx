/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfacePointVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:58 $
  Version:   $Revision: 1.4 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovSurfacePointVTKRenderMethod3D.h"
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
SurfacePointVTKRenderMethod3D
::SurfacePointVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("SurfaceSpatialObject");
}

/** Destructor */
SurfacePointVTKRenderMethod3D
::~SurfacePointVTKRenderMethod3D()
{
}

/** */
void 
SurfacePointVTKRenderMethod3D
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
SurfacePointVTKRenderMethod3D
::Update()
{
  ObjectListType::iterator            currentObject,lastObject;
 
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
  {

    if((*currentObject)->IsModified())
    {
      RemoveActor(*currentObject); // remove the actor before creating the new one.
      //vtk classes to hold points and set output
      vtkPoints    * newPoints = vtkPoints::New();
      vtkCellArray * newVerts  = vtkCellArray::New();
      vtkPolyData  * output    = vtkPolyData::New();

      //holds each point to be drawn
      float p[3];           //out

      SurfaceType::Pointer surface = dynamic_cast<SurfaceType *>((*currentObject)->GetObject());
      SurfacePointListType pointList = surface->GetPoints();

      SurfacePointListType::const_iterator it = pointList.begin();

      //get number of pnts to draw
      unsigned int numberOfPoints = pointList.size();

      //allocate the memory 
      newPoints->Allocate(numberOfPoints);
      newVerts->Allocate(newVerts->EstimateSize(1,numberOfPoints));
      newVerts->InsertNextCell(numberOfPoints);

      //point coordinates 
      float x,y,z;

      while(it!=pointList.end())
      {
        x = (float)((*it).GetPosition()[0])*surface->GetSpacing()[0];
        y = (float)((*it).GetPosition()[1])*surface->GetSpacing()[1];
        z = (float)((*it).GetPosition()[2])*surface->GetSpacing()[2];

        p[0] = x;
        p[1] = y;
        p[2] = z;

        newVerts->InsertCellPoint(newPoints->InsertNextPoint(p));
        it++;
      }

      //update 
      output->SetPoints(newPoints);
      output->SetVerts(newVerts);
    
      //for rendering
      vtkPolyDataMapper * mapper = vtkPolyDataMapper::New();
      vtkActor* actor = vtkActor::New();
 
      mapper->SetInput(output);
      actor->SetMapper(mapper);
      actor->GetProperty()->SetColor(1,0,0);
      (*currentObject)->AddVTKDefined(actor);
      m_ActorList.push_back(actor);
      m_ActorToBeAddedList.push_back(actor);
      (*currentObject)->UpdateMTime();

      //release memory
      newPoints->Delete();
      newVerts->Delete();
      mapper->Delete();
      output->Delete();
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
SurfacePointVTKRenderMethod3D
::PrintSelf (std::ostream &, Indent)  const
{
}

} // end of namespace sov
