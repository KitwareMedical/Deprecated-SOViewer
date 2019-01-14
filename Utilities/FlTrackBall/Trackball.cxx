/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: Trackball.cxx,v $
  Language:  C++
  Date:      $Date: 2005-09-27 15:00:37 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "Trackball.h"
#include <vnl/vnl_matrix.h>

/** Reset Trackball */
Trackball & Trackball::Reset()
{
  m_Mouse.Reset();
  m_Mouse.SetOglPosAndSize(-1,1,2,2);
  m_Size = 1; 
  m_Scale=1; 
  m_RotationSpeed = 20;
  m_Translation  = new VectorType(3);
  (*m_Translation)(0) = 0;
  (*m_Translation)(1) = 0;
  (*m_Translation)(2) = 0;
  m_Origin = new VectorType(3);
  (*m_Origin)(0) = 0;
  (*m_Origin)(1) = 0;
  (*m_Origin)(2) = 0;
  m_SpinQuater = new QuaternionType(0.0f,0.0f,0.0f,1.0f);
  m_RotQuater  = new QuaternionType(0.0f,0.0f,0.0f,1.0f);
  m_IsSpinning = false;
  m_IsChanged = true;
  m_Continous = false;
  return *this; 
}


/**
 * apply transformations according to current state
 * call this right before rendering the scene
 * (enclose both by gl push & pos matrix)
 */
void Trackball::DoTransform() 
{
  // Translate
  glTranslatef((GLfloat)(*m_Translation)(0), (GLfloat)(*m_Translation)(1), (GLfloat)(*m_Translation)(2));
  // Scale
  glScalef(m_Scale,m_Scale,m_Scale);
  // Rotate (do the computation by hand because vnl_quaternion rotation 
  //         matrix 4*4 is not implemented yet)          
  float m[16];
  double x=m_RotQuater->x();
  double y=m_RotQuater->y();
  double z=m_RotQuater->z();
  double w=m_RotQuater->r();

  m[0]=(float)(1.-2.*(y*y+z*z)); m[4]=(float)(   2.*(x*y-w*z)); m[8]=(float)(2.*(x*z+w*y));     m[12]=0.0f;
  m[1]=(float)(  2.*(x*y+w*z));  m[5]=(float)(1.-2.*(x*x+z*z)); m[9]=(float)(2.*(y*z-w*x));     m[13]=0.0f;
  m[2]=(float)(  2.*(x*z-w*y));  m[6]=(float)(  2.*(y*z+w*x));  m[10]=(float)(1.-2.*(x*x+y*y)); m[14]=0.0f;
  m[3]=0.0f;   m[7]=0.0f;m[11]=0.0f; m[15]=1.0f;
  
  glMultMatrixf(m);
  // Move Origin
  glTranslatef((GLfloat)(*m_Origin)(0), (GLfloat)(*m_Origin)(1), (GLfloat)(*m_Origin)(2));
}

/** update the trackball state using the current mouse state */
void Trackball::Update() 
{
  VectorType *v;

  // SPIN+DRAG (first two buttons) translates the origin of rotation
  if (m_Mouse.buttonsPressed == SPIN+DRAG) 
  {
    v = m_Origin; 
    m_Mouse.buttonsPressed = DRAG; 
  }
  else 
  {
    v = m_Translation;
  }
  
  switch (m_Mouse.buttonsPressed) 
  {
    case SPIN:
      Spin();  // updates Spin, which we then apply.
      (*m_RotQuater) = (*m_SpinQuater) * (*m_RotQuater);
      m_IsChanged = true;
      break;
    case DRAG:
      (*m_RotQuater) = (*m_SpinQuater) * (*m_RotQuater); 
      (*v)(0) += m_Mouse.dx;
      (*v)(1) += m_Mouse.dy; 
      m_IsChanged = true;
      break;
    case ZOOM: 
      m_Scale *= (float)(1+.002*(m_Mouse.dv));
      m_IsChanged = true;
      break;
    case SPIN+ZOOM:
      (*m_Translation)(0) = 0;
      (*m_Translation)(1) = 0;
      (*m_Translation)(2) = 0;
      (*m_Origin)(0) = 0;
      (*m_Origin)(1) = 0;
      (*m_Origin)(2) = 0;
      if(m_SpinQuater) {delete m_SpinQuater;}
      m_SpinQuater = new QuaternionType(0,0,0,1);
      if(m_RotQuater) {delete m_RotQuater;}
      m_RotQuater  = new QuaternionType(0,0,0,1);
      break;
    case DRAG+ZOOM:
      m_Scale=1;
      break;
    case SPIN+DRAG+ZOOM:
      (*m_Translation)(0) = 0;
      (*m_Translation)(1) = 0;
      (*m_Translation)(2) = 0;
      (*m_Origin)(0) = 0;
      (*m_Origin)(1) = 0;
      (*m_Origin)(2) = 0;
      if(m_SpinQuater) {delete m_SpinQuater;}
      m_SpinQuater = new QuaternionType(0,0,0,1);
      if(m_RotQuater) {delete m_RotQuater;}
      m_RotQuater  = new QuaternionType(0,0,0,1);
      m_Scale=1;
      break;
    default:
      if(m_Continous)
      {
        *m_RotQuater = (*m_SpinQuater) * (*m_RotQuater);
      }
  }   
} 
  

/**
 * Implementation of a virtual trackball.  
 * Original code by Gavin Bell, lots  of ideas from Thant Tessman 
 * and the August '88 issue of SigGraph's "Computer Graphics," pp. 121-129.
 *  
 * Simulate trackball spinning.  Project the mouse positions onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross product
 * of O P1 and O P2 (O is the center of the ball).  Note: This is a deformed
 * trackball -- it's a trackball in the center, but is deformed into a
 * hyperbolic sovid of rotation away from the center. 
 */
void Trackball::Spin() 
{
  // computes incremental spin quaternion associated with last mouse movement
  if (m_Mouse.du == 0 && m_Mouse.dv == 0) 
  {
    if(m_SpinQuater) { delete m_SpinQuater;}
    m_SpinQuater = new QuaternionType(0,0,0,1); // mouse didn't move => no rotation
    m_IsSpinning = false;
    return;
  }
  m_IsSpinning = true;

  float theta, t;
  VectorType p1(3);
  VectorType p2(3);
  VectorType axis(3);
  VectorType d(3);

  // Scale the old and new mouse positions to (-1, 1).
  //m_Mouse.TransformScreenToOgl();

  // First, figure out z-coordinates for projection of P1 and P2 to
  // the deformed sphere.
  // project 2D points onto deformed sphere top get 3D points
  p1(0)= m_Mouse.xOld;//mouse.uw/2;//
  p1(1)= m_Mouse.yOld;//mouse.vh/2;//mouse.yOld;
  p1(2)= DepthOnSphere(m_Mouse.xOld, m_Mouse.yOld);
  p2(0)= m_Mouse.x;
  p2(1)= m_Mouse.y;
  p2(2)= DepthOnSphere(m_Mouse.x, m_Mouse.y);
  
  // Now the axis of rotation.
  axis = itk_cross_3d(p1,p2);

  // Figure out how much to rotate around that axis.  
  d = (p1 - p2)/m_RotationSpeed;
  
  t = (float)(d.two_norm());
  if (t > 1.0) t = 1.0;
  if (t < -1.0) t = -1.0;
  theta = asin(t);

  // Return a rotation quaternion
  if(m_SpinQuater)
  {
    delete m_SpinQuater;
  }
  axis.normalize();
  m_SpinQuater = new QuaternionType(axis,theta);
}

/**
 * Project an x, y pair onto a sphere of radius r OR a hyperbolic sheet
 * if we are away from the center of the sphere.
 */
float Trackball::DepthOnSphere(float x, float y) 
{
  float d, t, z, r=m_Size;

  d = sqrt(x*x + y*y);
  if (d < r * 1/sqrt((double)2))
  {
    z = sqrt(r*r - d*d); // inside sphere
  } 
  else 
  {
    t = r / sqrt((float)2); // on hyperbola
    z = t*t / d;
  }
  return z;
} 

