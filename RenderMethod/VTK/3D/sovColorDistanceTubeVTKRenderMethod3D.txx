/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovColorDistanceTubeVTKRenderMethod3D.txx,v $
  Language:  C++
  Date:      $Date: 2005-09-25 01:15:57 $
  Version:   $Revision: 1.3 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "sovColorDistanceTubeVTKRenderMethod3D.h"
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include "vtkTubeFilter.h"
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>

#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h> 
#include <vtkFloatArray.h>
#include "vtkTubeFilterColor.h"
#include <vtkProperty.h>
#include <itkMinimumMaximumImageFilter.h>

namespace sov
{

/** Constructor */
template <class TImage>
ColorDistanceTubeVTKRenderMethod3D<TImage>
::ColorDistanceTubeVTKRenderMethod3D()
{
  m_SupportedTypeList->clear();
  AddSupportedType("VesselTubeSpatialObject");
  AddSupportedType("DTITubeSpatialObject");
  AddSupportedType("TubeSpatialObject");
  m_NumberOfSides = 5;
  m_ColorFactor = 10;
  CreateLookUpTable();
}

/** Destructor */
template <class TImage>
ColorDistanceTubeVTKRenderMethod3D<TImage>
::~ColorDistanceTubeVTKRenderMethod3D()
{
  m_LookUpTable->Delete();
}


/** */
template <class TImage>
void 
ColorDistanceTubeVTKRenderMethod3D<TImage>
::SetDistanceMap(ImageSpatialObjectType * image) 
{
  m_DistanceMap = image;

  typedef itk::MinimumMaximumImageFilter<TImage> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput(m_DistanceMap->GetImage());
  filter->Update();

  m_Maximum = filter->GetMaximum(); 

}


/** */
template <class TImage>
void
ColorDistanceTubeVTKRenderMethod3D<TImage>
::CreateLookUpTable()
{

       // Define the Color Table
  double ColorFactor;
  ColorFactor = 1; 
  double ColorOffset = 0;

  m_LookUpTable = vtkLookupTable::New();
  

  m_LookUpTable->SetNumberOfColors(257);

  
  int hueCase;
  float hue, sat, val, lx, ly, lz, frac, hinc, sinc, vinc, ainc;
  float rgba[4], alpha;
  float R,G,B;
 

  unsigned int NumberOfColors = 256;

  hinc = (0 - 0.6667)/(NumberOfColors-1);
  sinc = 0;
  vinc = 0;
  ainc = 0;

  for (unsigned int i=0; i < NumberOfColors; i++) 
  {
    hue = 0.6667 + i * hinc;
    sat = 1 + i * sinc;
    val = 1 + i * vinc;
    alpha = 1 + i * ainc;

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
template <class TImage>
double
ColorDistanceTubeVTKRenderMethod3D<TImage>
::GetLookUpTableIndex(float r,float g,float b)
{
  // Define the Color Table
  double ColorFactor = 1; 
  double ColorOffset = 0;

  int hueCase;
  float hue, sat, val, lx, ly, lz, frac, hinc, sinc, vinc, ainc;
  float rgba[4], alpha;
  float R,G,B;
 

  unsigned int NumberOfColors = 256;

  hinc = (0 - 0.6667)/(NumberOfColors-1);
  sinc = 0;
  vinc = 0;
  ainc = 0;


  // We want to minimize the distance
  float distance = 999999999;
  unsigned int index = 0;

  for (unsigned int i=0; i < NumberOfColors; i++) 
  {
    hue = 0.6667 + i * hinc;
    sat = 1 + i * sinc;
    val = 1 + i * vinc;
    alpha = 1 + i * ainc;

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
template <class TImage>
void
ColorDistanceTubeVTKRenderMethod3D<TImage>
::Update()
{

  ObjectListType::iterator            currentObject,lastObject;
  TubePointListType::iterator         currentTubePoint,lastTubePoint,test;
  TubeListType                        tubeList;
  
  currentObject = m_ObjectList.begin();
  lastObject = m_ObjectList.end();

  unsigned int tubeIndex=0;
  unsigned int pointIndex=0;


  for(; currentObject!=lastObject; currentObject++)
  {
    if((*currentObject)->IsModified())
    {

      RemoveActor(*currentObject); // remove the actor before creating the new one.

      typename TubeType::Pointer tube = static_cast<TubeType *>((*currentObject)->GetObject());
      int i;

      if(m_NumberOfSides < 3)
      {
        m_NumberOfSides = 3;
      }
      
      if(tube->GetNumberOfPoints() < 2)
      {
        std::cout << "Not enough points" << std::endl;
        return;
      }

      const double* spacing = tube->GetSpacing();

      //Step 1: copy skeleton points from a vessel into vtkPoints
      //vtkpoints assumes a triplet is coming so use pointer arithmetic
      //to jump to the next spot in a multidimensional array
      //TubePointListType::iterator  pnt = tube->GetPoints().begin();
      itk::Vector<float> lT(3);
      itk::Vector<float> lX(3);

      unsigned int nPoints = tube->GetNumberOfPoints();

      typedef typename TubeType::TubePointType TubePointType;
      const TubePointType* pt = static_cast<const TubePointType*>(tube->GetPoint(0)); 
        
      float lR;
      lX[0] = pt->GetPosition()[0]*spacing[0];
      lX[1] = pt->GetPosition()[1]*spacing[1];
      lX[2] = pt->GetPosition()[2]*spacing[2];
      lR = pt->GetRadius()*0.95*spacing[0];
      pt = static_cast<const TubePointType*>(tube->GetPoint(1)); 
      lT[0] = lX[0]-pt->GetPosition()[0]*spacing[0];
      lT[1] = lX[1]-pt->GetPosition()[1]*spacing[1];
      lT[2] = lX[2]-pt->GetPosition()[2]*spacing[2];
      lT.Get_vnl_vector().normalize();

      itk::Vector<float> cT(3);
      float tf;


      for(i=1;i<nPoints;i++)
      {
        pt = static_cast<const TubePointType*>(tube->GetPoint(i)); 
        cT[0] = lX[0]-pt->GetPosition()[0]*spacing[0];
        cT[1] = lX[1]-pt->GetPosition()[1]*spacing[1];
        cT[2] = lX[2]-pt->GetPosition()[2]*spacing[2];
        cT.Get_vnl_vector().normalize();
        tf = dot_product(cT.Get_vnl_vector(), lT.Get_vnl_vector());
        if(fabs(tf)<0.975 || fabs(pt->GetRadius()*0.95*spacing[0]-lR)>0.025*lR)
        {
          i++;
          lX[0] = pt->GetPosition()[0]*spacing[0];
          lX[1] = pt->GetPosition()[1]*spacing[1];
          lX[2] = pt->GetPosition()[2]*spacing[2];        
          lT[0] = lX[0]-pt->GetPosition()[0]*spacing[0];
          lT[1] = lX[1]-pt->GetPosition()[1]*spacing[1];
          lT[2] = lX[2]-pt->GetPosition()[2]*spacing[2];
          lT.Get_vnl_vector().normalize();
          lR = pt->GetRadius()*0.95*spacing[0];
        }
      }
      
      vtkPoints* vPoints = vtkPoints::New();
      vPoints->SetNumberOfPoints(nPoints);

      vtkFloatArray* vScalars = vtkFloatArray::New();
      vScalars->SetNumberOfTuples(nPoints);
      vtkFloatArray* vColorScalars = vtkFloatArray::New();
      vColorScalars->SetNumberOfTuples(nPoints);
      vtkFloatArray* vVectors = vtkFloatArray::New();
      vVectors->SetNumberOfTuples(3*nPoints);
      vVectors->SetNumberOfComponents(3);


      unsigned int pnt = 0;
      pt = static_cast<const TubePointType*>(tube->GetPoint(pnt)); 
      lX[0] = pt->GetPosition()[0]*spacing[0];
      lX[1] = pt->GetPosition()[1]*spacing[1];
      lX[2] = pt->GetPosition()[2]*spacing[2];

      lR = pt->GetRadius()*0.95*spacing[0];
      vPoints->SetPoint(0, (float)(pt->GetPosition()[0]*spacing[0]), (float)(pt->GetPosition()[1]*spacing[1]), (float)(pt->GetPosition()[2]*spacing[2]));
      vScalars->SetTuple1(0,pt->GetRadius()*0.95*spacing[0]);
      
      typename ImageType::IndexType index;
      bool valid = true;
      for(unsigned int ind=0;ind<3;ind++)
      {
        index[ind]=pt->GetPosition()[ind];
        if(  (index[ind] >= m_DistanceMap->GetImage()->GetLargestPossibleRegion().GetSize()[ind])
            || (index[ind] < 0)
          )
        {
          valid = false;
        }
      }

      if(valid)
      {
        vColorScalars->SetTuple1(0,1-(m_Maximum-(double)(m_DistanceMap->GetImage()->GetPixel(index)))/m_ColorFactor);
      }
      vVectors->SetTuple3(0,pt->GetRadius()*0.95*spacing[0],0,0);

      vtkSphereSource * sphereSource1 = vtkSphereSource::New();
      sphereSource1->SetCenter((float)(pt->GetPosition()[0]*spacing[0]), (float)(pt->GetPosition()[1]*spacing[1]), (float)(pt->GetPosition()[2]*spacing[2]));
      sphereSource1->SetRadius(pt->GetRadius()*0.95*spacing[0]);
  
      vtkPolyDataMapper *sphereMapper1 = vtkPolyDataMapper::New();
      sphereMapper1->SetInput(sphereSource1->GetOutput());
  
      vtkActor* sphere1 = vtkActor::New();
      sphere1->SetMapper(sphereMapper1);
  
      if(valid)
      {
        float *color = m_LookUpTable->GetTableValue(1-(m_Maximum-(double)(m_DistanceMap->GetImage()->GetPixel(index)))/m_ColorFactor);
        sphere1->GetProperty()->SetColor(color[0],
                                       color[1],
                                       color[2]); 
      }

      sphere1->GetProperty()->SetOpacity(1.0); 
      
      (*currentObject)->AddVTKDefined(sphere1);
      m_ActorToBeAddedList.push_back(sphere1);
      m_ActorList.push_back(sphere1);

      sphereMapper1->Delete();
      sphereSource1->Delete();
   
      pnt++;
      pt = static_cast<const TubePointType*>(tube->GetPoint(pnt)); 

      lT[0] = lX[0]-pt->GetPosition()[0]*spacing[0];
      lT[1] = lX[1]-pt->GetPosition()[1]*spacing[1];
      lT[2] = lX[2]-pt->GetPosition()[2]*spacing[2];
      lT.Get_vnl_vector().normalize();
          
      pnt++;  
      pt = static_cast<const TubePointType*>(tube->GetPoint(pnt)); 

      for(i=1; pnt<nPoints; pnt++)
      {
        pt = static_cast<const TubePointType*>(tube->GetPoint(pnt)); 
        cT[0] = lX[0]-pt->GetPosition()[0]*spacing[0];
        cT[1] = lX[1]-pt->GetPosition()[1]*spacing[1];
        cT[2] = lX[2]-pt->GetPosition()[2]*spacing[2];

        cT.Get_vnl_vector().normalize();
        tf = dot_product(cT.Get_vnl_vector(), lT.Get_vnl_vector());
        if(fabs(tf)<0.975 || fabs(pt->GetRadius()*0.95*spacing[0]-lR)>0.025*lR)
        {
          vPoints->SetPoint(i, (float)(pt->GetPosition()[0]*spacing[0]), (float)(pt->GetPosition()[1]*spacing[1]), (float)(pt->GetPosition()[2]*spacing[2]));
          vScalars->SetTuple1(i,pt->GetRadius()*0.95*spacing[0]);
          
          // perform bounds checking
          bool valid = true;
          for(unsigned int ind=0;ind<3;ind++)
          {
            index[ind]=pt->GetPosition()[ind];
            if(  (index[ind] >= m_DistanceMap->GetImage()->GetLargestPossibleRegion().GetSize()[ind])
               || (index[ind] < 0)
              )
            {
              valid = false;
            }
          }

          if(valid)
          {
             vColorScalars->SetTuple1(i,1-(m_Maximum-(double)(m_DistanceMap->GetImage()->GetPixel(index)))/m_ColorFactor);
          }
          vVectors->SetTuple3(i,pt->GetRadius()*0.95*spacing[0],0,0);

          i++;
          lX[0] = pt->GetPosition()[0]*spacing[0];
          lX[1] = pt->GetPosition()[1]*spacing[1];
          lX[2] = pt->GetPosition()[2]*spacing[2];
          pnt++;
          pt = static_cast<const TubePointType*>(tube->GetPoint(pnt)); 
          if(pnt != nPoints)
          {
            lT[0] = lX[0]-pt->GetPosition()[0]*spacing[0];
            lT[1] = lX[1]-pt->GetPosition()[1]*spacing[1];
            lT[2] = lX[2]-pt->GetPosition()[2]*spacing[2];
            lT.Get_vnl_vector().normalize();
            lR = pt->GetRadius()*0.95*spacing[0];
          }
          else
          {
            pnt--;
            pt = static_cast<const TubePointType*>(tube->GetPoint(pnt)); 
          }
        }
      }  
  
      pnt--;
      pt = static_cast<const TubePointType*>(tube->GetPoint(pnt)); 


      vPoints->SetPoint(i, (float)(pt->GetPosition()[0]*spacing[0]), (float)(pt->GetPosition()[1]*spacing[1]), (float)(pt->GetPosition()[2]*spacing[2]));
      vScalars->SetTuple1(i,pt->GetRadius()*0.95*spacing[0]);
      valid = true;
      for(unsigned int ind=0;ind<3;ind++)
      {
        index[ind]=pt->GetPosition()[ind];
        if(  (index[ind] >= m_DistanceMap->GetImage()->GetLargestPossibleRegion().GetSize()[ind])
             || (index[ind] < 0)
          )
        {
          valid = false;
        }
      }
      if(valid)
      {
        vColorScalars->SetTuple1(i,1-(m_Maximum-(double)(m_DistanceMap->GetImage()->GetPixel(index)))/m_ColorFactor);
      }
      vVectors->SetTuple3(i,pt->GetRadius()*0.95*spacing[0],0,0);

      vtkSphereSource * sphereSource2 = vtkSphereSource::New();
      sphereSource2->SetCenter((float)(pt->GetPosition()[0]*spacing[0]), (float)(pt->GetPosition()[1]*spacing[1]), (float)(pt->GetPosition()[2]*spacing[2]));
      sphereSource2->SetRadius(pt->GetRadius()*0.95*spacing[0]);
  
      vtkPolyDataMapper *sphereMapper2 = vtkPolyDataMapper::New();
      sphereMapper2->SetInput(sphereSource2->GetOutput());
  
      vtkActor* sphere2 = vtkActor::New();
      sphere2->SetMapper(sphereMapper2);
  
      if(valid)
      {
        float* color = m_LookUpTable->GetTableValue(1-(m_Maximum-(double)(m_DistanceMap->GetImage()->GetPixel(index)))/m_ColorFactor);
        sphere2->GetProperty()->SetColor(color[0],
                                       color[1],
                                       color[2]); 
      }
      
      sphere2->GetProperty()->SetOpacity(1.0); 

      (*currentObject)->AddVTKDefined(sphere2);
      m_ActorToBeAddedList.push_back(sphere2);
      m_ActorList.push_back(sphere2);

      sphereMapper2->Delete();
      sphereSource2->Delete();
  
      //Step 2: create a point id list (for a polyline this is just linear)
      int* pntIds = new int[nPoints];
      for (i = 0; i < nPoints; i++)
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
      float range[2];
      bool use_scalars = false;
      float min_scalar, max_scalar;
      vScalars->GetRange(range);
      min_scalar = range[0];
      max_scalar = range[1];
      if (min_scalar <= 0.0001) 
         min_scalar = 0.0001;
      if(max_scalar < min_scalar) 
         max_scalar = min_scalar;
      if (min_scalar != max_scalar) 
      {
        use_scalars = true;
        //vPData->GetPointData()->SetScalars(vScalars);
        vPData->GetPointData()->SetScalars(vColorScalars);
        vPData->GetPointData()->SetVectors(vVectors); 
      }

      //Step 7: remove any duplicate points from polydata. The tube filter
      //fails if any duplicates are present
      vtkCleanPolyData* vClean = vtkCleanPolyData::New();
      vClean->SetInput(vPData);

      //Step 8: make tubes. The number of sides per tube is set by nsides.
      //Even an nsides of 3 looks surprisingly good.
      vtkTubeFilterColor* vTFilter = vtkTubeFilterColor::New();
      vTFilter->SetNumberOfSides(m_NumberOfSides);
      vTFilter->SetInput(vClean->GetOutput());
      vTFilter->CappingOff();
      vTFilter->SetRadiusMin(range[0]);
      vTFilter->SetRadiusMax(range[1]);  

      if(use_scalars)
      {
        vTFilter->SetVaryRadiusToVaryRadiusByVector();
        vTFilter->SetRadius(min_scalar);   //this call sets min rad. Weird.
        vTFilter->SetRadiusFactor(max_scalar/min_scalar); //sets max rad. Weird
      }
      else
      {
        vTFilter->SetRadius(min_scalar);
        vTFilter->SetVaryRadiusToVaryRadiusOff();
      }
  
      //Step 9: create a mapper of the tube
      vtkPolyDataMapper* vMapper = vtkPolyDataMapper::New();
      vtkPolyData* vPDOutput = vtkPolyData::New();

      vPDOutput=vTFilter->GetOutput();
    
      // end of color map
      //vMapper->SetLookupTable(m_LookUpTable);
      vMapper->SetInput(vPDOutput);
      vMapper->ScalarVisibilityOn();    //interpret scalars as color command

      //Step 10: Add the mapper to the actor. You can now delete everything.
      //A matrix for the actor, colors, opacities, etc can be set by
      //the caller before or after this function is called.
      
      vtkActor* actor = vtkActor::New();
      (*currentObject)->AddVTKDefined(actor);
      actor->SetMapper(vMapper);
      m_ActorToBeAddedList.push_back(actor);
      m_ActorList.push_back(actor);
      (*currentObject)->UpdateMTime();
      vPoints->Delete();
      delete [] pntIds;
      vScalars->Delete();
      vPLine->Delete();
      vClean->Delete();
      vCA->Delete();
      vPData->Delete();
      vTFilter->Delete();
      vMapper->Delete();
      vColorScalars->Delete();
      vVectors->Delete();
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
template <class TImage>
void
ColorDistanceTubeVTKRenderMethod3D<TImage>
::PrintSelf (std::ostream &itkNotUsed(os), Indent itkNotUsed(indent)) const
{
}

} // end of namespace sov
