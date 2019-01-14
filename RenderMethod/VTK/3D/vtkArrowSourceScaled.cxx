/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkArrowSourceScaled.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkArrowSourceScaled.h"

#include "vtkAppendPolyData.h"
#include "vtkConeSource.h"
#include "vtkCylinderSource.h"
#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"
#include "vtkTransformFilter.h"

vtkCxxRevisionMacro(vtkArrowSourceScaled, "$Revision: 1.1.1.1 $");
vtkStandardNewMacro(vtkArrowSourceScaled);

vtkArrowSourceScaled::vtkArrowSourceScaled()
{
  this->TipResolution = 6;
  this->TipRadius = 0.1;
  this->TipLength = 0.35;
  this->ShaftResolution = 6;
  this->ShaftRadius = 0.03;
  this->Lenght = 1.0;
}


void vtkArrowSourceScaled::Execute()
{
  int piece, numPieces, ghostLevel;
  vtkPolyData *output = this->GetOutput();
  vtkCylinderSource *cyl = vtkCylinderSource::New();
  vtkTransform *trans0 = vtkTransform::New();
  vtkTransformFilter *tf0 = vtkTransformFilter::New();
  vtkConeSource *cone = vtkConeSource::New();
  vtkTransform *trans1 = vtkTransform::New();
  vtkTransformFilter *tf1 = vtkTransformFilter::New();
  vtkAppendPolyData *append = vtkAppendPolyData::New();

  piece = output->GetUpdatePiece();
  numPieces = output->GetUpdateNumberOfPieces();
  ghostLevel = output->GetUpdateGhostLevel();

  cyl->SetResolution(this->ShaftResolution);
  cyl->SetRadius(this->Lenght*this->ShaftRadius);
  cyl->SetHeight(this->Lenght*(1.0 - this->TipLength));
  cyl->SetCenter(0, this->Lenght*(1.0-this->TipLength)*0.5, 0.0);
  cyl->CappingOn();

  trans0->RotateZ(-90.0);
  tf0->SetTransform(trans0);
  tf0->SetInput(cyl->GetOutput());

  cone->SetResolution(this->TipResolution);
  cone->SetHeight(this->Lenght*this->TipLength);
  cone->SetRadius(this->Lenght*this->TipRadius);

  trans1->Translate(this->Lenght*(1.0-this->TipLength*0.5), 0.0, 0.0);
  tf1->SetTransform(trans1);
  tf1->SetInput(cone->GetOutput());

  append->AddInput(tf0->GetPolyDataOutput());
  append->AddInput(tf1->GetPolyDataOutput());

  if (output->GetUpdatePiece() == 0)
    {
    append->Update();
    output->ShallowCopy(append->GetOutput());
    }
  output->SetUpdatePiece(piece);
  output->SetUpdateNumberOfPieces(numPieces);
  output->SetUpdateGhostLevel(ghostLevel);

  cone->Delete();
  trans0->Delete();
  tf0->Delete();
  cyl->Delete();
  trans1->Delete();
  tf1->Delete();
  append->Delete();  
}


void vtkArrowSourceScaled::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "TipResolution: " << this->TipResolution << "\n";
  os << indent << "TipRadius: " << this->TipRadius << "\n";
  os << indent << "TipLength: " << this->TipLength << "\n";

  os << indent << "ShaftResolution: " << this->ShaftResolution << "\n";
  os << indent << "ShaftRadius: " << this->ShaftRadius << "\n";
}
