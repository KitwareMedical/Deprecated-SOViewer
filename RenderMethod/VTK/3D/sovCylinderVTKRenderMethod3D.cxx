/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovCylinderVTKRenderMethod3D.cxx,v $
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
#include "sovCylinderVTKRenderMethod3D.h"

#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>

namespace sov
{

CylinderVTKRenderMethod3D
::CylinderVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("CylinderSpatialObject");
}

CylinderVTKRenderMethod3D
::~CylinderVTKRenderMethod3D( void )
{
}



void
CylinderVTKRenderMethod3D
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
      CylinderType* CylinderSO = dynamic_cast<CylinderType *>((*currentObject)->GetObject());
   
      vtkPolyDataMapper *CylinderMapper = vtkPolyDataMapper::New();
      vtkActor* Cylinder = vtkActor::New();
    
     
      vtkCylinderSource* CylinderSource = vtkCylinderSource::New();
 
      Cylinder->GetProperty()->SetColor(CylinderSO->GetProperty()->GetRed(),
                                     CylinderSO->GetProperty()->GetGreen(),
                                     CylinderSO->GetProperty()->GetBlue()); 
        
      CylinderSource->SetResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
      CylinderSource->SetHeight(CylinderSO->GetHeight());
      CylinderSource->SetRadius(CylinderSO->GetRadius());

      Cylinder->GetProperty()->SetOpacity(1.0); 
      CylinderMapper->SetInput(CylinderSource->GetOutput());
      Cylinder->SetMapper(CylinderMapper);
      CylinderSource->Delete();
    
      (*currentObject)->AddVTKDefined(Cylinder);
      (*currentObject)->UpdateMTime();
      m_ActorToBeAddedList.push_back(Cylinder);
      m_ActorList.push_back(Cylinder);
      CylinderMapper->Delete();
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
CylinderVTKRenderMethod3D
::PrintSelf (std::ostream & itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

void
CylinderVTKRenderMethod3D
::Update()
{
  this->draw();
}

} // end of namespace sov
