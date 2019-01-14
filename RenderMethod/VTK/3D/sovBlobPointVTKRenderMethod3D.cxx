/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBlobPointVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:56 $
  Version:   $Revision: 1.2 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovBlobPointVTKRenderMethod3D.h"
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
#include <vtkContourFilter.h>

#include <itkBlobSpatialObjectToImageFilter.h>
#include "itkImageToVTKImageFilter.h"
#include <itkImage.h>
#include "itkRecursiveGaussianImageFilter.h"
#include <vtkSmoothPolyDataFilter.h>
#include <vtkDecimatePro.h>

namespace sov
{

/** Constructor */
BlobPointVTKRenderMethod3D
::BlobPointVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("BlobSpatialObject");
  m_Smoothness = 0.1; // beetween 0.01 and 0.1(smoother)
  m_Decimation = 0.6; // default = 0.2 (0.1 less decimation 1.0 fully decimated)
  m_Scale = 1;
}

/** Destructor */
BlobPointVTKRenderMethod3D
::~BlobPointVTKRenderMethod3D()
{
}

/** */
void 
BlobPointVTKRenderMethod3D
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
BlobPointVTKRenderMethod3D
::Update()
{
  //std::cout << "BlobPointVTKRenderMethod3D::Update() " << std::endl;
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

      BlobType::Pointer blob = dynamic_cast<BlobType *>((*currentObject)->GetObject());
      BlobPointListType pointList = blob->GetPoints();

      BlobPointListType::const_iterator it = pointList.begin();

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
        x = (float)((*it).GetPosition()[0])*blob->GetSpacing()[0];
        y = (float)((*it).GetPosition()[1])*blob->GetSpacing()[1];
        z = (float)((*it).GetPosition()[2])*blob->GetSpacing()[2];

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
BlobPointVTKRenderMethod3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}


} // end of namespace sov
