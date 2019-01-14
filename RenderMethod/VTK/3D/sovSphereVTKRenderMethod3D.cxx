/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSphereVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-13 15:24:49 $
  Version:   $Revision: 1.6 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovSphereVTKRenderMethod3D.h"

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
#include "vtkPoints.h"
#include "vtkImageData.h"

namespace sov
{

SphereVTKRenderMethod3D
::SphereVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("EllipseSpatialObject");
  m_UseTexture = false;
}

SphereVTKRenderMethod3D
::~SphereVTKRenderMethod3D( void )
{
}


void 
SphereVTKRenderMethod3D
::SetTextureFile(const char* name)
{
  m_TextureName = name;
  m_UseTexture = true;
}


void
SphereVTKRenderMethod3D
::draw( void )
{

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

      if(!m_UseTexture)
        {
        vtkSuperquadricSource* sphereSource = vtkSuperquadricSource::New();
        sphereSource->SetCenter(0, 0, 0);
        sphereSource->SetScale(sphereSO->GetSpacing()[0]*sphereSO->GetRadius()[0],
                               sphereSO->GetSpacing()[1]*sphereSO->GetRadius()[1],
                               sphereSO->GetSpacing()[2]*sphereSO->GetRadius()[2]);
        sphereSource->SetThetaResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
        sphereSource->SetPhiResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
        sphere->GetProperty()->SetColor(sphereSO->GetProperty()->GetRed(),
                                      sphereSO->GetProperty()->GetGreen(),
                                      sphereSO->GetProperty()->GetBlue()); 
        
        sphere->GetProperty()->SetOpacity((*currentObject)->GetProperty()->GetOpacity());
        sphereMapper->SetInput(sphereSource->GetOutput());
        sphere->SetMapper(sphereMapper);
        sphereSource->Delete();
        }
      else
        {
        /*vtkSuperquadricSource* sphereSource = vtkSuperquadricSource::New();
        sphereSource->SetCenter(0, 0, 0);
        sphereSource->SetScale(sphereSO->GetSpacing()[0]*sphereSO->GetRadius()[0],
                               sphereSO->GetSpacing()[1]*sphereSO->GetRadius()[1],
                               sphereSO->GetSpacing()[2]*sphereSO->GetRadius()[2]);
        sphereSource->SetThetaResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
        sphereSource->SetPhiResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
       
        vtkPlanes *planes = vtkPlanes::New();
        float pts[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        vtkPoints *points = vtkPoints::New();
          points->InsertPoint (0, pts);
          points->InsertPoint (1, pts + 3);
        float nrms[6] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0};

        planes->SetPoints (points);    
        vtkFloatArray *n = vtkFloatArray::New();
        n->SetNumberOfComponents(3);
        n->SetNumberOfTuples(2);
        n->SetTuple3(0,1.0,0.0,0.0);
        n->SetTuple3(1,0.0,1.0,0.0);
        planes->SetNormals (n);
        n->Delete();

        vtkImplicitTextureCoords *tcoords = vtkImplicitTextureCoords::New();
        tcoords->SetInput(sphereSource->GetOutput());
        tcoords->SetRFunction(planes);

        vtkDataSetMapper *outerMapper = vtkDataSetMapper::New();
        outerMapper->SetInput(tcoords->GetOutput());
        sphere->SetMapper(outerMapper);
      
        m_Texture->InterpolateOff();
        m_Texture->RepeatOff();
  
        sphere->SetTexture(m_Texture);
        sphereSource->Delete();*/   
        }

      (*currentObject)->AddVTKDefined(sphere);
      (*currentObject)->UpdateMTime();
      m_ActorToBeAddedList.push_back(sphere);
      m_ActorList.push_back(sphere);
      sphereMapper->Delete();
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
SphereVTKRenderMethod3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

void
SphereVTKRenderMethod3D
::Update()
{
  //std::cout << " SphereVTKRenderMethod3D : Update" << std::endl;
  this->draw();

}

} // end of namespace sov
