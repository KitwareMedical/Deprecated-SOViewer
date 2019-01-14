/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: itkCubicSplineInterpolation1D.cxx,v $
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
#include "itkCubicSplineInterpolation1D.h"

namespace itk
{


/** Constructor */
CubicSplineInterpolation1D::CubicSplineInterpolation1D()
{
}

/** Constructor */
CubicSplineInterpolation1D::CubicSplineInterpolation1D(MyListPoint data)
{

  m_Dim = data.size()-1;

  m_DataX = new float[m_Dim];  // initialization
  m_DataY = new float[m_Dim];
  m_H = new float[m_Dim];
  m_B = new float[m_Dim];
  m_Fsec = new float[m_Dim];
  
  m_Mat = Matrix(3, 3);

  MyListPoint::const_iterator it = data.begin();
  
  unsigned int i=0;
  while(it!=data.end())
  {
    m_DataX[i] = (*it)[0];  // reading the data
    m_DataY[i] = (*it)[1];
    ++it;
    ++i;
  }

  DefineCoeff();  // to compute all the coefficient
     
}


/** Destructor */
CubicSplineInterpolation1D::~CubicSplineInterpolation1D()
{
  delete [] m_DataX;
  delete []  m_DataY;
  delete []  m_H;
  delete []  m_B;
  delete []  m_Fsec;
}


void CubicSplineInterpolation1D::SetInput(MyListPoint data)
{
  m_Dim = data.size();

  m_DataX = new float[m_Dim];  // initialization
  m_DataY = new float[m_Dim];
  m_H = new float[m_Dim];
  m_B = new float[m_Dim];
  m_Fsec = new float[m_Dim]; 
  m_Mat = Matrix(3, 3);


  unsigned int i=0;
  MyListPoint::iterator it = data.begin();
  while(it!=data.end())
  {
  m_DataX[i] = (*it)[0];  // reading the data
  m_DataY[i] = (*it)[1];
  ++it;
  ++i;
  }

  DefineCoeff();

}

void CubicSplineInterpolation1D::SetInput(MyListTubePoint data)
{

  m_Dim = data.size()-1;

  m_DataX = new float[m_Dim];  // initialization
  m_DataY = new float[m_Dim];
  m_H = new float[m_Dim];
  m_B = new float[m_Dim];
  m_Fsec = new float[m_Dim];
  m_Radius = new float[m_Dim];
  
  m_Mat = Matrix(3, 3);

  MyListTubePoint::const_iterator it = data.begin();
  
  for(int i=0;i<m_Dim;i++)
  {
    m_DataX[i] = (*it).GetPosition()[0];  // reading the data
    m_DataY[i] = (*it).GetPosition()[1];
    m_Radius[i] = (*it).GetRadius();
    ++it;
  }

  DefineCoeff();

}

void CubicSplineInterpolation1D::DefineCoeff()
{


  // define the h coeff: step between 2 successive points
  m_H[0] = -11111;  //Not Usefull

  for(int i=1;i<m_Dim;i++)
  {
    m_H[i] = m_DataX[i]-m_DataX[i-1];
  }
  
  // define the B coeff:

  for(int i=1;i<m_Dim-1;i++)
  {
    m_B[i] = (6.0/m_H[i+1])*(m_DataY[i+1]-m_DataY[i]) + (6.0/m_H[i])*(m_DataY[i-1]-m_DataY[i]);
    
  }
  m_B[0] = 0;
  m_B[m_Dim-1] = 0;

  // defining the matrix to compute f'':


  for(int d=1;d<m_Dim-1;d++)
    {
    m_Mat.fill(0);
    for(int i=-1; i<2; i++)
      {
      if(i>-1)
        {
        m_Mat.put(i+1,i+1-1, m_H[d+i]);
        }
      if(d+i+1 < m_Dim)
        {
        m_Mat.put(i+1,i+1, 2.0*(m_H[d+i]+m_H[d+i+1]));
        }
      else
        {
        m_Mat.put(i+1,i+1, 2.0*(m_H[d+i]));
        }
      if(i<1)
        {
        m_Mat.put(i+1,i+1+1, m_H[d+i+1]);
        }
      }

    // computing f''
    MatrixInverse m_MatInverse(m_Mat);

    float value = 0;
    for(int j=0;j<3;j++)
      {
      value += m_MatInverse.inverse().get(1,j)*m_B[d+j-1];
      }
    
    m_Fsec[d] = value;
    }

//  m_Fsec[0] = m_Fsec[1];
//  m_Fsec[m_Dim-1] = m_Fsec[m_Dim-2];

    m_Fsec[0] = 0;
    m_Fsec[m_Dim-1] = 0;

  // F'' is now defined

}


float CubicSplineInterpolation1D::GetValueAtContinuousPosition(MyIndex index)
{
  // looking what is the position of the index:

  unsigned int i;
  if(m_DataX[1]<index[0] && index[0]<m_DataX[m_Dim-1])
    {
    i = FindPositionIndex(index,1,m_Dim-1);
    }
  else if(index[0]<=m_DataX[1])
    {
    i = 1;
    }
  else
    {
    i = m_Dim-1;
    }
  // Linear
 // double t = 1-(m_DataX[i]-index[0])/(m_DataX[i]-m_DataX[i-1]);
 // float value = t*m_DataY[i]+(1-t)*m_DataY[i-1];

  // do not try to understand the following code...
  float value = m_Fsec[i-1]*pow((float)(m_DataX[i]-index[0]),(float)3.0)/(6.0*m_H[i])
    +m_Fsec[i]*pow((float)(index[0]-m_DataX[i-1]),(float)3.0)/(6.0*m_H[i])
    +index[0]*((m_DataY[i]-m_DataY[i-1])/m_H[i]
    -(m_H[i]/6.0)*(m_Fsec[i]-m_Fsec[i-1])) + m_DataX[i]*(m_DataY[i-1]/m_H[i]
    -m_Fsec[i-1]*m_H[i]/6.0) - m_DataX[i-1]*(m_DataY[i]/m_H[i]-(m_H[i]/6.0)*m_Fsec[i]);

  
  return value;

}

float CubicSplineInterpolation1D::GetRadiusAtIndex(MyIndex index)
{

  unsigned int i;
  float t;
  float r;
  if(m_DataX[1]<index[0] && index[0]<m_DataX[m_Dim-1])
    {
    i = FindPositionIndex(index,1,m_Dim-1);
    }
  else if(index[0]<=m_DataX[1])
    {
    i = 1;
    }
  else
    {
    i = m_Dim-1;
    }

  t = 1-(m_DataX[i]-index[0])/(m_DataX[i]-m_DataX[i-1]);

  r = t*m_Radius[i]+(1-t)*m_Radius[i-1];

  return r;  

}


unsigned int CubicSplineInterpolation1D::FindPositionIndex(MyIndex index, unsigned int i1, unsigned int i2)
{

  /*

  if(i<=0 || i >= m_Dim)
    return -1;
  

  for(unsigned int j=1; j<m_Dim;j++)
  {
    
    if(m_DataX[j-1] <= index && index <= m_DataX[j])
    {
       return j;
    }
    
  
  }

  return -1;

*/

  // dichotomy

  unsigned int i = (unsigned int)(floor((float)(i1+i2+1)/2));
  
  if(m_DataX[i-1] <= index[0] && index[0] <= m_DataX[i])
    {
    return i;
    }
  else if(index[0] < m_DataX[i-1])
    {
    return FindPositionIndex(index,i1,i-1);
    }
  else
    {
    return FindPositionIndex(index,i,i2);
    }


}

}
