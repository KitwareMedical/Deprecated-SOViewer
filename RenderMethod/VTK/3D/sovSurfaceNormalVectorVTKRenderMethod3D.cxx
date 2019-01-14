/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSurfaceNormalVectorVTKRenderMethod3D.cxx,v $
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
#include "sovSurfaceNormalVectorVTKRenderMethod3D.h"
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
#include <vtkArrowSourceScaled.h>

namespace sov
{

/** Constructor */
SurfaceNormalVectorVTKRenderMethod3D
::SurfaceNormalVectorVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("SurfaceSpatialObject");
}

/** Destructor */
SurfaceNormalVectorVTKRenderMethod3D
::~SurfaceNormalVectorVTKRenderMethod3D()
{
}

/** */
void 
SurfaceNormalVectorVTKRenderMethod3D
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
SurfaceNormalVectorVTKRenderMethod3D
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

      // Add the normals
      it=pointList.begin();
      while(it!=pointList.end())
      {
      vtkPolyDataMapper *ArrowMapper = vtkPolyDataMapper::New();
      vtkActor* Arrow = vtkActor::New();
       double x = (float)((*it).GetPosition()[0])*surface->GetSpacing()[0];
       double  y = (float)((*it).GetPosition()[1])*surface->GetSpacing()[1];
       double  z = (float)((*it).GetPosition()[2])*surface->GetSpacing()[2];

      
      vtkArrowSourceScaled* ArrowSource = vtkArrowSourceScaled::New();

      Arrow->GetProperty()->SetColor(surface->GetProperty()->GetRed(),
                                     surface->GetProperty()->GetGreen(),
                                     surface->GetProperty()->GetBlue()); 
      ArrowSource->SetLenght((*it).GetNormal().GetNorm());

      Arrow->SetPosition(x,y,z);

      itk::CovariantVector<double,3> n = (*it).GetNormal();
      n.Normalize();

    double angley;
    double anglez = 0;
    
    #ifndef PI    
     const double PI = 4.0 * atan( 1.0 );
    #endif

    if(n[0] == 0.0)
      {
      if(n[1]>0.0)
        {
        anglez=PI/2;
        }
      else if(n[1]<0.0)
        {
        anglez=-PI/2;
        }
      }
    else
      {
      if(n[0]<0.0)
        {
        anglez = PI+atan(n[1]/n[0]);
        }
      else
        {
        anglez = atan(n[1]/n[0]);
        }
      }
    angley = -asin(n[2]);

    double anglex = 0;

         
      Arrow->SetOrientation(anglex*180.0/PI,angley*180.0/PI,anglez*180.0/PI);

      Arrow->GetProperty()->SetOpacity(1.0); 
      ArrowMapper->SetInput(ArrowSource->GetOutput());
      Arrow->SetMapper(ArrowMapper);
      ArrowSource->Delete();

      Arrow->GetProperty()->SetColor(0.0,0.0,1.0); //blue by default

      (*currentObject)->AddVTKDefined(Arrow);
      m_ActorList.push_back(Arrow);
      m_ActorToBeAddedList.push_back(Arrow);
      it++;
      for(unsigned int k=0;k<10;k++)
        {
        if(it!=pointList.end())
          {
          it++;
          }
        }

      ArrowMapper->Delete();
      }


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
SurfaceNormalVectorVTKRenderMethod3D
::PrintSelf (std::ostream &, Indent)  const
{
}

} // end of namespace sov
