/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovSparsePointVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:57 $
  Version:   $Revision: 1.5 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovSparsePointVTKRenderMethod3D.h"

#include <itkCovariantVector.h>
#include <itkPoint.h>
#include <vtkArrowSourceScaled.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>
#include <vtkSphereSource.h>
#include "vnl/vnl_det.h"
#include <itkEuler3DTransform.h>

namespace sov
{

#ifndef PI    
  const double PI = 4.0 * atan( 1.0 );
#endif

SparsePointVTKRenderMethod3D
::SparsePointVTKRenderMethod3D( void )
{
  m_SupportedTypeList->clear();
  AddSupportedType("SparsePointsSpatialObject");
}

SparsePointVTKRenderMethod3D
::~SparsePointVTKRenderMethod3D( void )
{
}



void
SparsePointVTKRenderMethod3D
::draw( void )
{
  ObjectListType::const_iterator            currentObject,lastObject;
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
    {
    if((*currentObject)->IsModified())
      {
      // remove the actor before creating the new one.
      RemoveActor(*currentObject); 
      SparsePointType* sparsePointSO = dynamic_cast<SparsePointType *>
                                             ((*currentObject)->GetObject());
   
      SparsePointListType::iterator pnt = sparsePointSO->GetPoints().begin();
      for(; pnt != sparsePointSO->GetPoints().end(); pnt++)
        {
        vtkActor* sparsePointActor = vtkActor::New();
        vtkPolyDataMapper *sparsePointMapper = vtkPolyDataMapper::New();

        if(pnt->GetNumberOfNormals() == 2)
          {
          itk::Point<double,3> center;
          center[0] = pnt->GetPosition()[0];
          center[1] = pnt->GetPosition()[1];
          center[2] = pnt->GetPosition()[2];

          itk::CovariantVector<double,3> n0;
          n0[0] = pnt->GetNormal(0)[0];
          n0[1] = pnt->GetNormal(0)[1];
          n0[2] = pnt->GetNormal(0)[2];

          itk::CovariantVector<double,3> n1;
          n1[0] = pnt->GetNormal(1)[0];
          n1[1] = pnt->GetNormal(1)[1];
          n1[2] = pnt->GetNormal(1)[2];

          itk::Point<double,3> centerT = sparsePointSO
                                      ->GetObjectToWorldTransform()
                                      ->TransformPoint(center);
          itk::CovariantVector<double,3> n0T = sparsePointSO
                                      ->GetObjectToWorldTransform()
                                      ->TransformCovariantVector(n0);
          itk::CovariantVector<double,3> n1T = sparsePointSO
                                      ->GetObjectToWorldTransform()
                                      ->TransformCovariantVector(n1);

          vtkPlaneSource * planeSource = vtkPlaneSource::New();

          const double dist = 2*pnt->GetScale(); 
          planeSource->SetOrigin(centerT[0]-n0T[0]*dist-n1T[0]*dist,
                                 centerT[1]-n0T[1]*dist-n1T[1]*dist,
                                 centerT[2]-n0T[2]*dist-n1T[2]*dist);
          planeSource->SetPoint1(centerT[0]+n0T[0]*dist-n1T[0]*dist,
                                 centerT[1]+n0T[1]*dist-n1T[1]*dist,
                                 centerT[2]+n0T[2]*dist-n1T[2]*dist);
          planeSource->SetPoint2(centerT[0]-n0T[0]*dist+n1T[0]*dist,
                                 centerT[1]-n0T[1]*dist+n1T[1]*dist,
                                 centerT[2]-n0T[2]*dist+n1T[2]*dist);

          sparsePointMapper->SetInput(planeSource->GetOutput());
          sparsePointActor->SetMapper(sparsePointMapper);
          planeSource->Delete();
          sparsePointActor->GetProperty()->SetColor(0, 1, 0);
          }
        else if(pnt->GetNumberOfNormals() == 1)
          {
          vtkArrowSourceScaled* ArrowSource = vtkArrowSourceScaled::New();
          sparsePointActor->GetProperty()->SetColor(1,0,0);
          itk::CovariantVector<double,3> n0 = pnt->GetNormal();
          itk::CovariantVector<double,3> n0T = sparsePointSO
                                      ->GetObjectToWorldTransform()
                                      ->TransformCovariantVector(n0);

          double angley;
    double anglez = 0;
    
    #ifndef PI    
     const double PI = 4.0 * atan( 1.0 );
    #endif

     itk::CovariantVector<double,3> dir = n0T;
     dir.Normalize();

    if(dir[0] == 0.0)
      {
      if(dir[1]>0.0)
        {
        anglez=PI/2;
        }
      else if(dir[1]<0.0)
        {
        anglez=-PI/2;
        }
      }
    else
      {
      if(n0T[0]<0.0)
        {
        anglez = PI+atan(dir[1]/dir[0]);
        }
      else
        {
        anglez = atan(dir[1]/dir[0]);
        }
      }
    angley = -asin(dir[2]);
               
          const double* spacing = sparsePointSO->GetIndexToObjectTransform()->GetScaleComponent();
          
          sparsePointActor->SetPosition((float)(pnt->GetPosition()[0]*spacing[0]), (float)(pnt->GetPosition()[1]*spacing[1]), (float)(pnt->GetPosition()[2]*spacing[2]));
         
          sparsePointActor->SetOrientation(0*180.0/PI,angley*180.0/PI,anglez*180.0/PI);

          ArrowSource->SetLenght(n0.GetNorm());
          sparsePointActor->GetProperty()->SetOpacity(1.0);
          vtkPolyDataMapper *ArrowMapper = vtkPolyDataMapper::New();
          ArrowMapper->SetInput(ArrowSource->GetOutput());
          sparsePointActor->SetMapper(ArrowMapper);
          ArrowSource->Delete();
          }
        else
          {
          vtkSphereSource * sphereSource = vtkSphereSource::New();
  
          sphereSource->SetCenter(
              pnt->GetPosition()[0] * sparsePointSO->GetSpacing()[0],
              pnt->GetPosition()[1] * sparsePointSO->GetSpacing()[1],
              pnt->GetPosition()[2] * sparsePointSO->GetSpacing()[2]);
          sphereSource->SetRadius(pnt->GetScale()*2.0);
          sphereSource->SetThetaResolution(5);
          sphereSource->SetPhiResolution(5);

          sparsePointMapper->SetInput(sphereSource->GetOutput());
          sparsePointActor->SetMapper(sparsePointMapper);
          sphereSource->Delete();
          sparsePointActor->GetProperty()->SetColor(0, 1, 0);
          }

        
        sparsePointActor->GetProperty()->SetOpacity(
                                           sparsePointSO
                                             ->GetProperty()->GetAlpha()); 
        (*currentObject)->AddVTKDefined(sparsePointActor);
        (*currentObject)->UpdateMTime();
        m_ActorToBeAddedList.push_back(sparsePointActor);
        m_ActorList.push_back(sparsePointActor);
        sparsePointMapper->Delete();
        }
      }

    if((*currentObject)->IsTransformModified())
      {
      ApplyTransform(* currentObject);
      (*currentObject)->UpdateTransformMTime();
      }
    }
}

void
SparsePointVTKRenderMethod3D
::PrintSelf (std::ostream & itkNotUsed(os), Indent itkNotUsed(indent)) const
{

}

void
SparsePointVTKRenderMethod3D
::Update()
{
  this->draw();
}

} // end of namespace sov
