/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSphereVTKRenderMethodSlicer.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-13 15:24:49 $
  Version:   $Revision: 1.5 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovSphereVTKRenderMethodSlicer.h"

#include "vtkSphereSource.h"
#include "vtkSuperquadricSource.h"
#include "vtkTexturedSphereSource.h"
#include "vtkPlaneSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkPlanes.h"
#include "vtkImplicitTextureCoords.h"
#include "vtkDataSetMapper.h"
#include "vtkProperty.h"
#include "vtkFloatArray.h"
#include "vtkExtractPolyDataGeometry.h"
#include "vtkPlane.h"
#include "vtkCutter.h"
#include "vtkImageData.h"


namespace sov
{

SphereVTKRenderMethodSlicer
::SphereVTKRenderMethodSlicer( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("EllipseSpatialObject");
  m_UseTexture = false;
}

SphereVTKRenderMethodSlicer
::~SphereVTKRenderMethodSlicer( void )
{
}


void 
SphereVTKRenderMethodSlicer
::SetTextureFile(const char* name)
{
  m_TextureName = name;
  m_UseTexture = true;
}


/** Draw function */
void SphereVTKRenderMethodSlicer
::draw( void )
{
 // std::cout << "SphereVTKRenderMethodSlicer:Draw" << std::endl;
  vtkCutter *cut = vtkCutter::New(); 
  cut->SetInput(*(m_PolyDataList.begin())); 
  vtkPlane* slicePlane = vtkPlane::New();
  slicePlane->SetOrigin(-100,-100,m_Depth);
  cut->SetCutFunction (slicePlane); 

  cut->Update();
  vtkActor* actor = static_cast<vtkActor*>(*(m_ActorList.begin()));
  static_cast<vtkPolyDataMapper*>(actor->GetMapper())->SetInput(cut->GetOutput());
  actor->GetProperty()->SetColor(1.0,0.0,0.0);
  actor->GetProperty()->SetOpacity(1.0);
}


/** Update function */
void SphereVTKRenderMethodSlicer
::Update( void )
{
  //std::cout << " SphereVTKRenderMethodSlicer : draw"  << std::endl;
 
  
  if(m_UseTexture)
  {
    vtkBMPReader *reader = vtkBMPReader::New();
    reader->SetFileName(m_TextureName);
    m_Texture = vtkTexture::New();
    m_Texture->SetInput(reader->GetOutput());
    m_Texture->InterpolateOn();
  }


  ObjectListType::const_iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
  { 
    if((*currentObject)->IsModified())
    {
      RemoveActor(*currentObject); // remove the actor before creating the new one.
      SphereType* sphereSO = dynamic_cast<SphereType *>((*currentObject)->GetObject());
   
      vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
      vtkActor* sphere = vtkActor::New();
    
    
        
      //vtkSphereSource* sphereSource = vtkSphereSource::New();
        vtkSuperquadricSource* sphereSource = vtkSuperquadricSource::New();
        sphereSource->SetCenter(0, 0, 0);
        sphereSource->SetScale(sphereSO->GetRadius()[0],sphereSO->GetRadius()[1],sphereSO->GetRadius()[2]);
        //sphereSource->SetRadius(sphereSO->GetRadius()[0]);
        sphereSource->SetThetaResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
        sphereSource->SetPhiResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
 
        //sphereSource->SetCenter(0,0,0);
        //sphereSource->SetRadius(sphereSO.GetRadius()[0]);

        sphere->GetProperty()->SetColor(sphereSO->GetProperty()->GetRed(),
                                      sphereSO->GetProperty()->GetGreen(),
                                      sphereSO->GetProperty()->GetBlue()); 
        
        sphere->GetProperty()->SetOpacity(1.0); 

        m_PolyDataList.push_back(sphereSource->GetOutput());
     

/*
        switch(this->m_Orientation)
          {
          case 0:
          break;
          case 1:

          break;
          case 2:

          break;
          }
*/
       sphereMapper->SetInput(sphereSource->GetOutput());
        sphere->SetMapper(sphereMapper);
        sphereSource->Delete();
      
      (*currentObject)->AddVTKDefined(sphere);
      (*currentObject)->UpdateMTime();
      m_ActorToBeAddedList.push_back(sphere);

      m_ActorList.push_back(sphere);
      sphereMapper->Delete();
    }
/*
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
*/
    this->draw();
  }
}

void
SphereVTKRenderMethodSlicer
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

/*void
SphereVTKRenderMethodSlicer
::Update()
{
  //std::cout << " SphereVTKRenderMethodSlicer : Update" << std::endl;
  this->draw();
}*/

} // end of namespace sov
