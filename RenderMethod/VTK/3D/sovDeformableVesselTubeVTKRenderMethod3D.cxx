/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovDeformableVesselTubeVTKRenderMethod3D.cxx,v $
  Language:  C++
  Date:      $Date: 2006-03-01 00:04:22 $
  Version:   $Revision: 1.5 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovDeformableVesselTubeVTKRenderMethod3D.h"
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkTubeFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>

#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h> 
#include <vtkFloatArray.h>
#include <vtkProperty.h>
#include <vtkArrowSourceScaled.h>

#ifndef vtkFloatingPointType
# define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif

namespace sov
{

/** Constructor */
DeformableVesselTubeVTKRenderMethod3D
::DeformableVesselTubeVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("DeformableVesselTubeSpatialObject");
  this->CreateLookUpTable();
}

/** Destructor */
DeformableVesselTubeVTKRenderMethod3D
::~DeformableVesselTubeVTKRenderMethod3D()
{
  m_LookUpTable->Delete();
}

/** */
void 
DeformableVesselTubeVTKRenderMethod3D
::draw( void )
{
}


/** */
void 
DeformableVesselTubeVTKRenderMethod3D
::CreateLookUpTable()
{

       // Define the Color Table
  //double ColorFactor;
  //ColorFactor = 1; 
  //double ColorOffset = 0;

  m_LookUpTable = vtkLookupTable::New();
  

  m_LookUpTable->SetNumberOfColors(257);

  
  int hueCase;
  float hue, sat, val, lx, ly, lz, frac, hinc, sinc, vinc;
  float rgba[4];
  float R,G,B;
 

  unsigned int NumberOfColors = 256;

  hinc = (0 - 0.6667)/(NumberOfColors-1);
  sinc = 0;
  vinc = 0;

  for (unsigned int i=0; i < NumberOfColors; i++) 
  {
    hue = 0.6667 + i * hinc;
    sat = 1 + i * sinc;
    val = 1 + i * vinc;

    hueCase = (int)(hue * 6);
    frac = 6*hue - hueCase;
    lx = val*(1.0 - sat);
    ly = val*(1.0 - sat*frac);
    lz = val*(1.0 - sat*(1.0 - frac));

    switch (hueCase) 
    {
      // 0<hue<1/6 
      case 0:
      case 6:
        rgba[0] = val;
        rgba[1] = lz;
        rgba[2] = lx;
        break;
      // 1/6<hue<2/6
      case 1:
        rgba[0] = ly;
        rgba[1] = val;
        rgba[2] = lx;
        break;
      // 2/6<hue<3/6
      case 2:
        rgba[0] = lx;
        rgba[1] = val;
        rgba[2] = lz;
        break;
      // 3/6<hue/4/6
      case 3:
        rgba[0] = lx;
        rgba[1] = ly;
        rgba[2] = val;
        break;
      // 4/6<hue<5/6
      case 4:
        rgba[0] = lz;
        rgba[1] = lx;
        rgba[2] = val;
        break;
      // 5/6<hue<1
      case 5:
        rgba[0] = val;
        rgba[1] = lx;
        rgba[2] = ly;
        break;
    }

    R = (unsigned char) 
       ((float)127.5*(1.0+(float)cos((1.0-(double)rgba[0])*3.141593)));
    G = (unsigned char)
        ((float)127.5*(1.0+(float)cos((1.0-(double)rgba[1])*3.141593)));
    B = (unsigned char)
        ((float)127.5*(1.0+(float)cos((1.0-(double)rgba[2])*3.141593)));
    m_LookUpTable->SetTableValue(i,R/255,G/255,B/255,1);
  }

  m_LookUpTable->SetTableValue(256,1,0,0,0);

}

/** Get the index of the lookup table given R,G,B values */
double
DeformableVesselTubeVTKRenderMethod3D
::GetLookUpTableIndex(float r,float g,float b)
{
  // Define the Color Table
  //double ColorFactor = 1; 
  //double ColorOffset = 0;

  int hueCase;
  float hue, sat, val, lx, ly, lz, frac, hinc, sinc, vinc;
  float rgba[4];
  float R,G,B;
 

  unsigned int NumberOfColors = 256;

  hinc = (0 - 0.6667)/(NumberOfColors-1);
  sinc = 0;
  vinc = 0;


  // We want to minimize the distance
  float distance = 999999999;
  unsigned int index = 0;

  for (unsigned int i=0; i < NumberOfColors; i++) 
  {
    hue = 0.6667 + i * hinc;
    sat = 1 + i * sinc;
    val = 1 + i * vinc;

    hueCase = (int)(hue * 6);
    frac = 6*hue - hueCase;
    lx = val*(1.0 - sat);
    ly = val*(1.0 - sat*frac);
    lz = val*(1.0 - sat*(1.0 - frac));

    switch (hueCase) 
    {
      // 0<hue<1/6 
      case 0:
      case 6:
        rgba[0] = val;
        rgba[1] = lz;
        rgba[2] = lx;
        break;
      // 1/6<hue<2/6
      case 1:
        rgba[0] = ly;
        rgba[1] = val;
        rgba[2] = lx;
        break;
      // 2/6<hue<3/6
      case 2:
        rgba[0] = lx;
        rgba[1] = val;
        rgba[2] = lz;
        break;
      // 3/6<hue/4/6
      case 3:
        rgba[0] = lx;
        rgba[1] = ly;
        rgba[2] = val;
        break;
      // 4/6<hue<5/6
      case 4:
        rgba[0] = lz;
        rgba[1] = lx;
        rgba[2] = val;
        break;
      // 5/6<hue<1
      case 5:
        rgba[0] = val;
        rgba[1] = lx;
        rgba[2] = ly;
        break;
    }

    R = (unsigned char) 
       ((float)127.5*(1.0+(float)cos((1.0-(double)rgba[0])*3.141593)));
    G = (unsigned char)
        ((float)127.5*(1.0+(float)cos((1.0-(double)rgba[1])*3.141593)));
    B = (unsigned char)
        ((float)127.5*(1.0+(float)cos((1.0-(double)rgba[2])*3.141593)));

    float Red = float(R)/255;
    float Green = float(G)/255;
    float Blue = float(B)/255;

    double dist = sqrt((Red-r)*(Red-r)+(Green-g)*(Green-g)+(Blue-b)*(Blue-b));

    if(dist<distance)
    {
      distance = dist;
      index =i;
    }

  }
 
  return double(0.99-(double(index))/256);


}



/** Rebuild the display list using the object attached 
 * to this render method. */
void
DeformableVesselTubeVTKRenderMethod3D
::Update()
{

  ObjectListType::iterator            currentObject,lastObject;

  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  for(; currentObject!=lastObject; currentObject++)
  {
    if((*currentObject)->IsModified())
    {
      // Remove all precedent actors
      RemoveActor(*currentObject); // remove the actor before creating the new one 

      TubeType::Pointer tube = static_cast<TubeType *>((*currentObject)->GetObject());
      unsigned int i;

      const double* spacing = tube->GetIndexToObjectTransform()->GetScaleComponent();

      //Step 1: copy skeleton points from a vessel into vtkPoints
      //vtkpoints assumes a triplet is coming so use pointer arithmetic
      //to jump to the next spot in a multidimensional array
      unsigned int nPoints = tube->GetNumberOfPoints();
      vtkPoints* vPoints = vtkPoints::New();
      vPoints->SetNumberOfPoints(nPoints);

      vtkFloatArray* vScalars = vtkFloatArray::New();
      vScalars->SetNumberOfTuples(nPoints);  
   
      for(i=0;i<nPoints;i++)
        {
        const TubeType::TubePointType* pt = static_cast<const TubeType::TubePointType*>(tube->GetPoint(i));
        vPoints->SetPoint(i, (float)(pt->GetPosition()[0]*spacing[0]), (float)(pt->GetPosition()[1]*spacing[1]), (float)(pt->GetPosition()[2]*spacing[2]));
        vScalars->SetTuple1(i,GetLookUpTableIndex(pt->GetColor()[0],pt->GetColor()[1],pt->GetColor()[2]));

        if(pt->GetDisplacement().GetNorm() > 0.0)
          {
          // define an arrow to represent the displacement
          vtkPolyDataMapper *ArrowMapper = vtkPolyDataMapper::New();    
          vtkActor* Arrow = vtkActor::New();
          vtkArrowSourceScaled* ArrowSource = vtkArrowSourceScaled::New();

          Arrow->GetProperty()->SetColor(0,0,1.0);
          ArrowSource->SetLenght(pt->GetDisplacement().GetNorm());
          Arrow->SetPosition((float)(pt->GetPosition()[0]*spacing[0]),(float)(pt->GetPosition()[1]*spacing[1]), (float)(pt->GetPosition()[2]*spacing[2]));
          
          double angley;
          double anglez = 0;
    
          #ifndef PI    
           const double PI = 4.0 * atan( 1.0 );
          #endif

          if(pt->GetDisplacement()[0] == 0.0)
            {
            if(pt->GetDisplacement()[1]>0.0)
              {
              anglez=PI/2;
              }
            else if(pt->GetDisplacement()[1]<0.0)
              {
              anglez=-PI/2;
              }
            }
          else
            {
            if(pt->GetDisplacement()[0]<0.0)
              {
              anglez = PI+atan(pt->GetDisplacement()[1]/pt->GetDisplacement()[0]);
              }
            else
              {
              anglez = atan(pt->GetDisplacement()[1]/pt->GetDisplacement()[0]);
              }
            }
          angley = -asin(pt->GetDisplacement()[2]);
          Arrow->SetOrientation(0.0,-angley*180/PI,-anglez*180/PI);
          
          Arrow->GetProperty()->SetOpacity(1.0); 
          ArrowMapper->SetInput(ArrowSource->GetOutput());
          Arrow->SetMapper(ArrowMapper);
          ArrowSource->Delete();
        
          (*currentObject)->AddVTKDefined(Arrow);
          m_ActorToBeAddedList.push_back(Arrow);
          m_ActorList.push_back(Arrow);
          ArrowMapper->Delete();
          }
        }
  

      //Step 2: create a point id list (for a polyline this is just linear
      vtkIdType* pntIds = new vtkIdType[nPoints];
      for (i = 0; i < (int)nPoints; i++)
        {
        pntIds[i] = i;
        }

      //Step3: create a polyline from the points and pt id list
      vtkPolyLine* vPLine = vtkPolyLine::New();
      vPLine->Initialize(nPoints, pntIds, vPoints);

      //Step 4: convert this to a cellarray (required for input to polydata)
      vtkCellArray* vCA = vtkCellArray::New();
      vCA->InsertNextCell(vPLine); 

      //Step 5: create a scalar array that indicates the radius at each
      //skeleton point. Vtk's way of setting radius is screwy: it fails if every
      //point has the same radius. It also uses a minimum radius  (called radius)
      //and a max radius (defined by scale factor). In order to get this to work, 
      //you need to find the min and max of your vessel radii--if the same, later
      //set a constant radius in the tube filter. If not the same, you need to 
      //define the min radius and the ratio max/min. If you send these params,
      //the tube will end up with proper radius settings. Weird.

      //Step 6: convert to polydata (required for input to anything else)
      vtkPolyData* vPData = vtkPolyData::New();
      vPData->SetLines(vCA);
      vPData->SetPoints(vPoints);
      vPData->GetPointData()->SetScalars(vScalars);
        
      //Step 7: remove any duplicate points from polydata. The tube filter
      //fails if any duplicates are present
      vtkCleanPolyData* vClean = vtkCleanPolyData::New();
      vClean->SetInput(vPData);

      //Step 9: create a mapper of the tube
      vtkPolyDataMapper* vMapper = vtkPolyDataMapper::New();
      vMapper->SetInput(vClean->GetOutput());   
      vMapper->ScalarVisibilityOn();    //interpret scalars as color command

      //Step 10: Add the mapper to the actor. You can now delete everything.
      //A matrix for the actor, colors, opacities, etc can be set by
      //the caller before or after this function is called.
      
      vtkActor* actor = vtkActor::New();
      actor->SetMapper(vMapper);

      actor->GetProperty()->SetColor(tube->GetProperty()->GetRed(),
                                     tube->GetProperty()->GetGreen(),
                                     tube->GetProperty()->GetBlue()); 
      actor->GetProperty()->SetOpacity(1.0); 
      
      (*currentObject)->AddVTKDefined(actor);
      m_ActorList.push_back(actor);
      m_ActorToBeAddedList.push_back(actor);
      (*currentObject)->UpdateMTime();

      vPoints->Delete();
      delete [] pntIds;
      vScalars->Delete();
      vPLine->Delete();
      vCA->Delete();
      vClean->Delete();
      vPData->Delete();
      vMapper->Delete();
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
DeformableVesselTubeVTKRenderMethod3D
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}

} // end of namespace sov
