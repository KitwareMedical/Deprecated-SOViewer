/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: itkCubicSplineInterpolation3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-11-16 03:55:43 $
  Version:   $Revision: 1.4 $
  Author:    Pierre Fillard (fillard@cs.unc.edu)

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkCubicSplineInterpolation3D.h"

namespace itk
{

CubicSplineInterpolation3D::CubicSplineInterpolation3D()
{
  m_NPointsInterpolated = 100;
  m_R = 1;
  m_G = 0;
  m_B = 0;
}


CubicSplineInterpolation3D::~CubicSplineInterpolation3D()
{
}

void CubicSplineInterpolation3D::SetNbPointsInterpolated(float n)
{
  m_NPointsInterpolated = n;
}

void CubicSplineInterpolation3D::SetRed(float r)
{
  m_R = r;
}

void CubicSplineInterpolation3D::SetGreen(float g)
{
  m_G = g;
}

void CubicSplineInterpolation3D::SetBlue(float b)
{
  m_B = b;
}

void CubicSplineInterpolation3D::SetInput(MyListPoint3D data)
{

  MyListPoint2D dataX;
  MyListPoint2D dataY;
  MyListPoint2D dataZ;
  MyPoint2D Pt;

  MyListPoint1D param = Parametric(data);

  MyListPoint1D::iterator it1D = param.begin();
  MyListPoint3D::iterator it3D = data.begin();

  while(it1D!=param.end())
  {

    Pt[0] = (*it1D)[0];
    Pt[1] = (*it3D)[0];
    dataX.push_back(Pt);
    Pt[1] = (*it3D)[1];
    dataY.push_back(Pt);
    Pt[1] = (*it3D)[2];
    dataZ.push_back(Pt);

    ++it1D;
    ++it3D;
  }

  m_1DSplineX = CubicSplineInterpolation1D(dataX);
  m_1DSplineY = CubicSplineInterpolation1D(dataY);
  m_1DSplineZ = CubicSplineInterpolation1D(dataZ);
  
}


void CubicSplineInterpolation3D::SetInput(TubePointList data)
{

  TubePointList2D dataX;
  TubePointList2D dataY;
  TubePointList2D dataZ;
  MyListPoint3D data2;

  
  TubePointList::iterator it = data.begin();
  while(it!=data.end())
  {
    MyPoint3D tmp;
    tmp[0] = (*it).GetPosition()[0];
    tmp[1] = (*it).GetPosition()[1];
    tmp[2] = (*it).GetPosition()[2];
    data2.push_back(tmp);
    ++it;
  }  

  MyListPoint1D param = Parametric(data2);
  MyListPoint1D::iterator it1D = param.begin();
  it = data.begin();

  // SUBTLE: the following loop access the end data point twice
  //   This is by design - it allows the spline to have a smooth
  //   taper at the end
  while(it1D!=param.end())
  {
    if(it == data.end())
      {
      --it;
      }

    TubePoint2D Pt;
    Pt.SetPosition((*it1D)[0],(*it).GetPosition()[0]);
    Pt.SetRadius((*it).GetRadius());
    dataX.push_back(Pt);


    Pt.SetPosition((*it1D)[0],(*it).GetPosition()[1]);
    Pt.SetRadius((*it).GetRadius());
    dataY.push_back(Pt);

    
    Pt.SetPosition((*it1D)[0],(*it).GetPosition()[2]);
    Pt.SetRadius((*it).GetRadius());
    dataZ.push_back(Pt);


    ++it1D;
    ++it;
  }

  m_1DSplineX.SetInput(dataX);
  m_1DSplineY.SetInput(dataY);
  m_1DSplineZ.SetInput(dataZ);
 
}


void CubicSplineInterpolation3D::SetInput(TubeType3D* tube)
{
  
  TubePointList2D dataX;
  TubePointList2D dataY;
  TubePointList2D dataZ;
  MyListPoint3D data2;

  unsigned int i;
  for(i=0;i<tube->GetNumberOfPoints();i++);
    {
    MyPoint3D tmp;
    tmp[0] = tube->GetPoint(i)->GetPosition()[0];
    tmp[1] = tube->GetPoint(i)->GetPosition()[1];
    tmp[2] = tube->GetPoint(i)->GetPosition()[2];
    data2.push_back(tmp);
    }  

  MyListPoint1D param = Parametric(data2);
  MyListPoint1D::iterator it1D = param.begin();
  
  i = 0;
  while(it1D!=param.end())
  {
    TubePoint2D Pt;
    Pt.SetPosition((*it1D)[0],tube->GetPoint(i)->GetPosition()[0]);
    Pt.SetRadius(static_cast<const TubePoint*>(tube->GetPoint(i))->GetRadius());
    dataX.push_back(Pt);
    

    Pt.SetPosition((*it1D)[0],tube->GetPoint(i)->GetPosition()[1]);
    Pt.SetRadius(static_cast<const TubePoint*>(tube->GetPoint(i))->GetRadius());
    dataY.push_back(Pt);
      
    
    Pt.SetPosition((*it1D)[0],tube->GetPoint(i)->GetPosition()[2]);
    Pt.SetRadius(static_cast<const TubePoint*>(tube->GetPoint(i))->GetRadius());
    dataZ.push_back(Pt);
        

    ++it1D;
    i++;
  }

  m_1DSplineX.SetInput(dataX);
  m_1DSplineY.SetInput(dataY);
  m_1DSplineZ.SetInput(dataZ);
}


CubicSplineInterpolation3D::MyListPoint3D CubicSplineInterpolation3D::GetInterpolatedPointList()
{

  MyListPoint3D ListPointsInterpolated;
  MyPoint3D PointInterpolated;
  MyContinuousIndex index;

  for(unsigned int i=0; i<m_NPointsInterpolated; i++)
  {
  
    float tmp[1] = {float(i)/(float)(m_NPointsInterpolated-1)};
    index = MyContinuousIndex(tmp);

    PointInterpolated[0] = m_1DSplineX.GetValueAtContinuousPosition(index);
    PointInterpolated[1] = m_1DSplineY.GetValueAtContinuousPosition(index);
    PointInterpolated[2] = m_1DSplineZ.GetValueAtContinuousPosition(index);
          
    ListPointsInterpolated.push_back(PointInterpolated);
    
  }

  return ListPointsInterpolated;

}

CubicSplineInterpolation3D::TubePointList CubicSplineInterpolation3D::GetInterpolatedTubePointList()
{

  TubePointList ListTubePointsInterpolated;
  TubePoint TubePointInterpolated;
  MyContinuousIndex index;
  
  
  for(unsigned int i=0; i<(unsigned int)m_NPointsInterpolated; i++)
  {
  
    index[0] = float(i)/(m_NPointsInterpolated-1);

      
    TubePointInterpolated.SetPosition(m_1DSplineX.GetValueAtContinuousPosition(index),m_1DSplineY.GetValueAtContinuousPosition(index)
      ,m_1DSplineZ.GetValueAtContinuousPosition(index));
    
    float r = m_1DSplineX.GetRadiusAtIndex(index);

    TubePointInterpolated.SetRadius(r);
    TubePointInterpolated.SetRed(m_R);
    TubePointInterpolated.SetGreen(m_G);
    TubePointInterpolated.SetBlue(m_B);
    ListTubePointsInterpolated.push_back(TubePointInterpolated);
    
  }

  return ListTubePointsInterpolated;

}




CubicSplineInterpolation3D::MyListPoint1D CubicSplineInterpolation3D::Parametric(MyListPoint3D data)
{

  int size = data.size();
 
  float * n = new float[size-1];
  float * nc = new float[size-1];
  float * vx = new float[size-1];
  float * vy = new float[size-1];
  float * vz = new float[size-1];
  float * datax = new float[size];
  float * datay = new float[size];
  float * dataz = new float[size];


  MyListPoint3D::iterator  it = data.begin();
  unsigned int i=0;
  while(it!=data.end())
  {
    datax[i] = (*it)[0];
    datay[i] = (*it)[1];
    dataz[i] = (*it)[2];
    ++i;
    ++it;
    
  }



  for(int i=0;i<size-1;i++)
  {
    vx[i] = datax[i+1]-datax[i];
    vy[i] = datay[i+1]-datay[i];
    vz[i] = dataz[i+1]-dataz[i];

    n[i] = sqrt(pow(vx[i],2)+pow(vy[i],2)+pow(vz[i],2));

    if(i==0) nc[i]=n[i];
    else nc[i] = nc[i-1]+n[i];  
  
  }

  
  MyListPoint1D param;
  MyPoint1D Pt;

  Pt[0] = 0;
  param.push_back(Pt);
  
  m_Length = nc[size-2];
  
  for(int i=0;i<size-1;i++)
  {
    Pt[0] = nc[i]/nc[size-2];
    param.push_back(Pt);
  }

  Pt[0] = 1.0;
  param.push_back(Pt);
  
  delete [] n;
  delete [] nc;
  delete [] vx;
  delete [] vy;
  delete [] vz;
  delete [] datax;
  delete [] datay;
  delete [] dataz;
 
  return param;

}

}
