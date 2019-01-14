/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovBoxVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-08-31 00:35:21 $
  Version:   $Revision: 1.4 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovBoxVTKRenderMethod3D.h"

#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>

namespace sov
{

BoxVTKRenderMethod3D
::BoxVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("BoxSpatialObject");
}

BoxVTKRenderMethod3D
::~BoxVTKRenderMethod3D( void )
{
}



void
BoxVTKRenderMethod3D
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
      RectangleType* RectangleSO = dynamic_cast<RectangleType *>((*currentObject)->GetObject());
   
      vtkPolyDataMapper *RectangleMapper = vtkPolyDataMapper::New();
      vtkActor* Rectangle = vtkActor::New();
     
      vtkCubeSource* cubeSource = vtkCubeSource::New();
 
      Rectangle->GetProperty()->SetColor(RectangleSO->GetProperty()->GetRed(),
                                     RectangleSO->GetProperty()->GetGreen(),
                                     RectangleSO->GetProperty()->GetBlue()); 
      
      cubeSource->SetXLength(RectangleSO->GetSize()[0]);
      cubeSource->SetYLength(RectangleSO->GetSize()[1]);
      cubeSource->SetZLength(RectangleSO->GetSize()[2]);

      Rectangle->GetProperty()->SetOpacity(RectangleSO->GetProperty()->GetAlpha()); 
      RectangleMapper->SetInput(cubeSource->GetOutput());
      Rectangle->SetMapper(RectangleMapper);
      cubeSource->Delete();
    
      (*currentObject)->AddVTKDefined(Rectangle);
      (*currentObject)->UpdateMTime();
      m_ActorToBeAddedList.push_back(Rectangle);
      m_ActorList.push_back(Rectangle);
      RectangleMapper->Delete();
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
BoxVTKRenderMethod3D
::PrintSelf (std::ostream & itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

void
BoxVTKRenderMethod3D
::Update()
{
  this->draw();
}

} // end of namespace sov
