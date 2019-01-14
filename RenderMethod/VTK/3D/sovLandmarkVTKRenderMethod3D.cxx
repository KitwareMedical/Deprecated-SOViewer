/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLandmarkVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:57 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovLandmarkVTKRenderMethod3D.h"

#include <vtkSphereSource.h>
#include <vtkTexturedSphereSource.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkPlanes.h>
#include <vtkImplicitTextureCoords.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>

namespace sov
{

LandmarkVTKRenderMethod3D
::LandmarkVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("LandmarkSpatialObject");
}

LandmarkVTKRenderMethod3D
::~LandmarkVTKRenderMethod3D( void )
{
}





void
LandmarkVTKRenderMethod3D
::draw( void )
{
  ObjectListType::const_iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
  { 
    if((*currentObject)->IsModified())
      { 
      RemoveActor(*currentObject); // remove the actor before creating the new one.
      LandmarkType* sphereSO = dynamic_cast<LandmarkType *>((*currentObject)->GetObject());
      LandmarkPointListType::iterator  pnt = sphereSO->GetPoints().begin();
  
      for(; pnt!=sphereSO->GetPoints().end(); pnt++)
      {

      vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
      vtkActor* sphere = vtkActor::New();
   
      vtkSphereSource* sphereSource = vtkSphereSource::New();
      sphereSource->SetCenter(pnt->GetPosition()[0]*sphereSO->GetSpacing()[0], 
                              pnt->GetPosition()[1]*sphereSO->GetSpacing()[1],  
                              pnt->GetPosition()[2]*sphereSO->GetSpacing()[2]);
      sphereSource->SetRadius(1);
      sphereSource->SetThetaResolution(10);
      sphereSource->SetPhiResolution(10);
 
      sphere->GetProperty()->SetColor(pnt->GetRed(),
                                      pnt->GetGreen(),
                                      sphereSO->GetProperty()->GetBlue()); 
        
      sphere->GetProperty()->SetOpacity(1.0); 
      sphereMapper->SetInput(sphereSource->GetOutput());
      sphere->SetMapper(sphereMapper);
      sphereSource->Delete();

      (*currentObject)->AddVTKDefined(sphere);
      (*currentObject)->UpdateMTime();
      m_ActorToBeAddedList.push_back(sphere);
      m_ActorList.push_back(sphere);
      sphereMapper->Delete();
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

void
LandmarkVTKRenderMethod3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

void
LandmarkVTKRenderMethod3D
::Update()
{
  this->draw();
}

} // end of namespace sov
