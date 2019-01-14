/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovArrowVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-03-16 21:59:45 $
  Version:   $Revision: 1.4 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovArrowVTKRenderMethod3D.h"

#include <vtkArrowSourceScaled.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>
#include <vtkArrowSource.h>

#include <sovVisualArrowProperty.h>

namespace sov
{

ArrowVTKRenderMethod3D
::ArrowVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("ArrowSpatialObject");
}

ArrowVTKRenderMethod3D
::~ArrowVTKRenderMethod3D( void )
{
}



void
ArrowVTKRenderMethod3D
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
      ArrowType* ArrowSO = dynamic_cast<ArrowType *>((*currentObject)->GetObject());
   
      vtkPolyDataMapper *ArrowMapper = vtkPolyDataMapper::New();
      vtkActor* Arrow = vtkActor::New();

      vtkArrowSourceScaled* ArrowSource = vtkArrowSourceScaled::New();

      Arrow->GetProperty()->SetColor(ArrowSO->GetProperty()->GetRed(),
                                     ArrowSO->GetProperty()->GetGreen(),
                                     ArrowSO->GetProperty()->GetBlue()); 
      ArrowSource->SetLenght(ArrowSO->GetLenght());

      if(!(static_cast<VisualArrowProperty *>((*currentObject)->GetProperty())->GetScale()))
        {
        ArrowSource->SetTipRadius(ArrowSource->GetTipRadius()/ArrowSO->GetLenght());
        ArrowSource->SetTipLength(ArrowSource->GetTipLength()/ArrowSO->GetLenght());
        ArrowSource->SetShaftRadius(ArrowSource->GetShaftRadius()/ArrowSO->GetLenght());
        }

      Arrow->GetProperty()->SetOpacity(1.0); 
      ArrowMapper->SetInput(ArrowSource->GetOutput());
      Arrow->SetMapper(ArrowMapper);
      ArrowSource->Delete();
    
      (*currentObject)->AddVTKDefined(Arrow);
      (*currentObject)->UpdateMTime();
      m_ActorToBeAddedList.push_back(Arrow);
      m_ActorList.push_back(Arrow);
      ArrowMapper->Delete();
      ApplyTransform(* currentObject); // need to force the transform for some reasons...
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
ArrowVTKRenderMethod3D
::PrintSelf (std::ostream & itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

void
ArrowVTKRenderMethod3D
::Update()
{
  this->draw();
}

} // end of namespace sov
