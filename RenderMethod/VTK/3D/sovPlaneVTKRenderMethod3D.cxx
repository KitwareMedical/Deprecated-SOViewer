/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovPlaneVTKRenderMethod3D.cxx,v $
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
#include "sovPlaneVTKRenderMethod3D.h"

#include "vtkPlaneSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkImplicitTextureCoords.h"
#include "vtkPlanes.h"
#include "vtkDataSetMapper.h"
#include "vtkProperty.h"
#include "vtkImageData.h"

namespace sov
{

PlaneVTKRenderMethod3D
::PlaneVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("PlaneSpatialObject");
  m_UseTexture = false;
}

PlaneVTKRenderMethod3D
::~PlaneVTKRenderMethod3D( void )
{
}


void 
PlaneVTKRenderMethod3D
::SetTextureFile(const char* name)
{
  m_TextureName = name;
  m_UseTexture = true;
}


void
PlaneVTKRenderMethod3D
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

      PlaneType* PlaneSO = dynamic_cast<PlaneType *>((*currentObject)->GetObject());
   
      vtkPolyDataMapper *PlaneMapper = vtkPolyDataMapper::New();
      vtkActor* Plane = vtkActor::New();
    
      char* textureName = (*currentObject)->GetProperty()->GetTextureFilename();
    
      if(textureName)
      {
        vtkBMPReader *reader = vtkBMPReader::New();
        reader->SetFileName(textureName);
        m_Texture = vtkTexture::New();
        m_Texture->SetInput(reader->GetOutput());
        m_Texture->InterpolateOn();
      }


      itk::Point<double,3> center;
      center[0]=0;
      center[1]=0;
      center[2]=0;

      itk::Point<double,3> center_transformed = PlaneSO->GetObjectToWorldTransform()->TransformPoint(center);

      if(!textureName)
      { 

        vtkPlaneSource* planeSource = vtkPlaneSource::New();
        planeSource->SetOrigin(0,0,0);
        planeSource->SetPoint1(PlaneSO->GetLowerPoint()[0],PlaneSO->GetLowerPoint()[1],PlaneSO->GetLowerPoint()[2]);
        planeSource->SetPoint2(PlaneSO->GetUpperPoint()[0],PlaneSO->GetUpperPoint()[1],PlaneSO->GetUpperPoint()[2]);
        Plane->GetProperty()->SetColor(PlaneSO->GetProperty()->GetRed(),
                                      PlaneSO->GetProperty()->GetGreen(),
                                      PlaneSO->GetProperty()->GetBlue());
        Plane->GetProperty()->SetOpacity((*currentObject)->GetProperty()->GetOpacity()); 
        PlaneMapper->SetInput(planeSource->GetOutput());
        Plane->SetMapper(PlaneMapper);
        planeSource->Delete();
      }
      else
      {
        vtkPlaneSource* planeSource = vtkPlaneSource::New();
        planeSource->SetOrigin(0,0,0);
        planeSource->SetPoint1(PlaneSO->GetLowerPoint()[0],PlaneSO->GetLowerPoint()[1],PlaneSO->GetLowerPoint()[2]);
        planeSource->SetPoint2(PlaneSO->GetUpperPoint()[0],PlaneSO->GetUpperPoint()[1],PlaneSO->GetUpperPoint()[2]);
        PlaneMapper->SetInput(planeSource->GetOutput());
        Plane->SetMapper(PlaneMapper);
        Plane->SetTexture(m_Texture);
        planeSource->Delete();     
      }
    
      (*currentObject)->AddVTKDefined(Plane);
      (*currentObject)->UpdateMTime();
      m_ActorList.push_back(Plane);
      m_ActorToBeAddedList.push_back(Plane);
      PlaneMapper->Delete();
    }

    if((*currentObject)->IsTransformModified())
    {
      ApplyTransform(* currentObject);
      (*currentObject)->UpdateTransformMTime();
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

  }
}

void
PlaneVTKRenderMethod3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

void
PlaneVTKRenderMethod3D
::Update()
{
  //std::cout << " PlaneVTKRenderMethod3D : Update" << std::endl;
  this->draw();
}

} // end of namespace sov
