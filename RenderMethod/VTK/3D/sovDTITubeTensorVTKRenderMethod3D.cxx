/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovDTITubeTensorVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-03-01 00:04:22 $
  Version:   $Revision: 1.10 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovDTITubeTensorVTKRenderMethod3D.h"
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include "vtkTubeFilter.h"
#include <vtkPolyDataMapper.h>
#include <vtkSuperquadricSource.h>

#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h> 
#include <vtkFloatArray.h>
#include "vtkTubeFilterColor.h"
#include <vtkProperty.h>
#include "vnl/vnl_vector_fixed.h"
#include "vnl/vnl_matrix_fixed.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"
#include "vnl/vnl_math.h"
#include "vnl/vnl_diag_matrix.h"
#include "vnl/vnl_det.h"
#include <itkEuler3DTransform.h>
#include <vtkMatrix4x4.h>
#include <sovVisualTubeProperty.h>

#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif

namespace sov
{

/** Constructor */
DTITubeTensorVTKRenderMethod3D
::DTITubeTensorVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("DTITubeSpatialObject");
  m_NumberOfSides = 5;
}

/** Destructor */
DTITubeTensorVTKRenderMethod3D
::~DTITubeTensorVTKRenderMethod3D()
{
}

/** */
void 
DTITubeTensorVTKRenderMethod3D
::draw( void )
{
}

/** Rebuild the display list using the object attached 
 * to this render method. */
void
DTITubeTensorVTKRenderMethod3D
::Update()
{

  ObjectListType::iterator            currentObject,lastObject;
  TubeListType                        tubeList;
  
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();
  
  for(; currentObject!=lastObject; currentObject++)
    {
    if((*currentObject)->IsModified())
      {
      #ifndef PI    
        const double PI = 4.0 * atan( 1.0 );
      #endif
      // Remove all precedent actors
      RemoveActor(*currentObject); // remove the actor before creating the new one 

      TubeType::Pointer tube = static_cast<TubeType *>((*currentObject)->GetObject());
            
      int i;

      if(m_NumberOfSides < 3)
        {
        m_NumberOfSides = 3;
        }
     

      const double* spacing = tube->GetIndexToObjectTransform()->GetScaleComponent();

      float ellipseFactor = static_cast<VisualTubeProperty *>((*currentObject)->GetProperty())->GetEllipseFactor();

      bool useColorPoints = (*currentObject)->GetProperty()->GetUsePointColor();

        //Step 1: copy skeleton points from a vessel into vtkPoints
        //vtkpoints assumes a triplet is coming so use pointer arithmetic
        //to jump to the next spot in a multidimensional array      
        int nPoints = tube->GetNumberOfPoints();

        vtkPoints* vPoints = vtkPoints::New();
        vPoints->SetNumberOfPoints(nPoints);
        vtkFloatArray* vScalars = vtkFloatArray::New();
        vScalars->SetNumberOfTuples(nPoints);
        vtkFloatArray* vColorScalars = vtkFloatArray::New();
        vColorScalars->SetNumberOfTuples(nPoints);
        vtkFloatArray* vVectors = vtkFloatArray::New();
        vVectors->SetNumberOfTuples(3*nPoints);
        vVectors->SetNumberOfComponents(3);        

        for(i=0;i<nPoints;i+=1)
          {   
          const TubeType::TubePointType* pt = static_cast<const TubeType::TubePointType*>(tube->GetPoint(i)); 

          // Compute the tensor value
          // Fill Tensor - Compute Eigensystem
          vnl_matrix_fixed<double,3,3> myTensor;
          myTensor.put(0,0, pt->GetTensorMatrix()[0]);
          myTensor.put(0,1, pt->GetTensorMatrix()[1]);
          myTensor.put(0,2, pt->GetTensorMatrix()[2]);
          myTensor.put(1,0, pt->GetTensorMatrix()[1]);
          myTensor.put(1,1, pt->GetTensorMatrix()[3]);
          myTensor.put(1,2, pt->GetTensorMatrix()[4]);
          myTensor.put(2,0, pt->GetTensorMatrix()[2]);
          myTensor.put(2,1, pt->GetTensorMatrix()[4]);
          myTensor.put(2,2, pt->GetTensorMatrix()[5]);

          vnl_symmetric_eigensystem<double>* myEigenSystem;
          myEigenSystem = new vnl_symmetric_eigensystem<double>(myTensor);
          vnl_vector_fixed<double,3> ev1(myEigenSystem->get_eigenvector(2));
          vnl_vector_fixed<double,3> ev2(myEigenSystem->get_eigenvector(1));
          vnl_vector_fixed<double,3> ev3(myEigenSystem->get_eigenvector(0));
          float vp1 = myEigenSystem->get_eigenvalue(2);
          float vp2 = myEigenSystem->get_eigenvalue(1);
          float vp3 = myEigenSystem->get_eigenvalue(0);
          
          delete myEigenSystem;


          vtkActor* sphere = vtkActor::New();
          vtkSuperquadricSource* sphereSource = vtkSuperquadricSource::New();          

          sphereSource->SetScale(vp1*ellipseFactor,
                                 vp2*ellipseFactor,
                                 vp3*ellipseFactor);

          // Now set the orientation of the tensor
          itk::Matrix<double,3,3> rotationMatrix;
          rotationMatrix[0][0] = ev1[0];
          rotationMatrix[0][1] = ev2[0];
          rotationMatrix[0][2] = ev3[0];
          rotationMatrix[1][0] = ev1[1];
          rotationMatrix[1][1] = ev2[1];
          rotationMatrix[1][2] = ev3[1];
          rotationMatrix[2][0] = ev1[2];
          rotationMatrix[2][1] = ev2[2];
          rotationMatrix[2][2] = ev3[2];

          // if det = -1, multiply rotationMatrix by -1
          double myDet = vnl_det(rotationMatrix.GetVnlMatrix());
          if (myDet<0.0) 
            {
            rotationMatrix.GetVnlMatrix() = (-1.0) * rotationMatrix.GetVnlMatrix();
            }

 
          typedef itk::Euler3DTransform<double> TransformType;
          TransformType::Pointer transform = TransformType::New();

          try
            {
            transform->SetRotationMatrix(rotationMatrix);
            }
           catch( itk::ExceptionObject &e )
            {
            std::cout << "Error: " << e.GetDescription() << std::endl;
            }
          
          sphere->SetPosition((float)(pt->GetPosition()[0]*spacing[0]), (float)(pt->GetPosition()[1]*spacing[1]), (float)(pt->GetPosition()[2]*spacing[2]));
         
          sphere->SetOrientation(transform->GetAngleX()*180.0/PI,transform->GetAngleY()*180.0/PI,transform->GetAngleZ()*180.0/PI);

          sphereSource->SetThetaResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
          sphereSource->SetPhiResolution(static_cast<int>((*currentObject)->GetProperty()->GetRenderingResolution()));
          
          if(useColorPoints)
            {
            sphere->GetProperty()->SetColor(pt->GetRed(),
                                        pt->GetGreen(),
                                        pt->GetBlue());
            }
          else
            {
            sphere->GetProperty()->SetColor(tube->GetProperty()->GetRed(),
                                        tube->GetProperty()->GetGreen(),
                                        tube->GetProperty()->GetBlue());             
            }
          sphere->GetProperty()->SetOpacity((*currentObject)->GetProperty()->GetOpacity());
          vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
          sphereMapper->SetInput(sphereSource->GetOutput());
          sphere->SetMapper(sphereMapper);    
            
              
          (*currentObject)->AddVTKDefined(sphere);
          (*currentObject)->UpdateMTime();
          m_ActorToBeAddedList.push_back(sphere);
          m_ActorList.push_back(sphere);

          sphereMapper->Delete();
          sphereSource->Delete();
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

/** Print the method */
void
DTITubeTensorVTKRenderMethod3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}

} // end of namespace sov
