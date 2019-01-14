/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSpherePhantomRenderMethod.cxx,v $
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
#include "sovSpherePhantomRenderMethod.h"
#include <GhostWrap.h>

namespace sov
{

SpherePhantomRenderMethod
::SpherePhantomRenderMethod( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("EllipseSpatialObject");
}

SpherePhantomRenderMethod
::~SpherePhantomRenderMethod( void )
{
}

void
SpherePhantomRenderMethod
::Update( void )
{
  std::cout << " SpherePhantomRenderMethod : Update"  << std::endl;
 
  ObjectListType::const_iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
  { 
    if((*currentObject)->IsModified())
    { 
      //RemoveActor(*currentObject); // remove the actor before creating the new one.
      SphereType* sphereSO = dynamic_cast<SphereType *>((*currentObject)->GetObject());
   
       /* 
        vtkSphereSource* sphereSource = vtkSphereSource::New();
        sphereSource->SetCenter(0, 0, 0);
        sphereSource->SetRadius(sphereSO->GetRadius()[0]);
        sphereSource->SetThetaResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
        sphereSource->SetPhiResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
 
        //sphereSource->SetCenter(0,0,0);
        //sphereSource->SetRadius(sphereSO.GetRadius()[0]);
*/
      // If the object does not exist we create it 
      gwpSphere *mySphere = static_cast<gwpSphere*>(*((*currentObject)->GetVTKDefinedList().begin()));
      if((*currentObject)->GetVTKDefinedList().size() == 0)
        {
        std::cout << "Creating new sphere" << std::endl;
        mySphere = new gwpSphere;
        (*currentObject)->AddVTKDefined(mySphere);
        m_ActorList.push_back(mySphere);
        m_ActorToBeAddedList.push_back(mySphere);
        }
        
      mySphere->setRadius(sphereSO->GetRadius()[0]);
      
      (*currentObject)->UpdateMTime();
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
SpherePhantomRenderMethod
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}



} // end of namespace sov
