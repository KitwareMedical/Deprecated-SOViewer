/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: Trackball.h,v $
  Language:  C++
  Date:      $Date: 2005-08-02 14:57:49 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef Trackball_h
#define Trackball_h

#include <vnl/vnl_vector.h>
#include <vnl/vnl_quaternion.h>
#include <vnl/vnl_quaternion.txx>
#include "Mouse.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include "TrackballExport.h"

#define SPIN (FL_BUTTON1)
#define DRAG (FL_BUTTON2)
#define ZOOM (FL_BUTTON3)
#define BUTTON_MASK (FL_BUTTON1 | FL_BUTTON2 | FL_BUTTON3)

#include <vxl_version.h>
#if VXL_VERSION_DATE_FULL > 20040406
# include <vnl/vnl_cross.h>
# define itk_cross_3d vnl_cross_3d
#else
# define itk_cross_3d cross_3d
#endif

class TrackBall_EXPORT Trackball 
{
public:

  /** Standard Typedefs */
  typedef vnl_vector<double> VectorType;
  typedef vnl_quaternion<double> QuaternionType;

  /** Constructor */
  Trackball (Mouse & m) : m_Mouse(m) { Reset(); }

  /** Reset the trackball */
  Trackball & Reset();

  /** Size of the virtual trackball as fraction of window 
   *  usefull range: .5 to 1.5, 1.0 default */
  void  SetSize(float size) { m_Size = size; assert (m_Size>0); }
  float GetSize(void)  { return m_Size; }
  
  /** Set the uniform scene scale factor */
  void  SetScale(float scale) { m_Scale = scale; }
  
  /** Get the scale factor */
  float GetScale(void) { return m_Scale; }
  
  /** trackball/scene center of rotation */
  void  SetOrigin(float x, float y, float z) { (*m_Origin)(0)=x,(*m_Origin)(1)=y,(*m_Origin)(2)=z; }
  
  /** Get the origin */
  const VectorType & GetOrigin(void) { return *m_Origin;  }
  
  /** Trackball/scene rotation (angle assumed in [-pi, +pi]) */
  void  SetRotation(double angle, VectorType & axis)
  { 
    if(m_RotQuater) {  delete m_RotQuater; }
    m_RotQuater = new QuaternionType(axis,angle);                                                                  
  }
  /** Get the Rotation quaternion*/
  const QuaternionType & GetRotation(void) {return *m_RotQuater;}
  
  /** Trackball/scene translation */
  void  SetTranslation(float x, float y, float z) { (*m_Translation)(0)=x,(*m_Translation)(1)=y,(*m_Translation)(2)=z;}
  
  /** Get the translation */
  const VectorType & GetTranslation(void) {return *m_Translation;}
  
  /** Do the transformation */
  void DoTransform();
  
  /** Update the scene */
  void Update();
  
  /** Spin the object */
  void Spin();
  
  /** Project 2D points onto a sphere to obtai 3D points*/
  float DepthOnSphere(float x, float y);

public:

  Mouse & m_Mouse;                // screen pixel coordinates, origin at (0,0)
  float m_Size;                   // Size of the spere to project on
  float m_Scale;                  // uniform object scale factor
  float m_RotationSpeed;          // Speed of the rotation
  VectorType* m_Translation;      // translation
  VectorType* m_Origin;           // point to rotate about
  QuaternionType* m_SpinQuater;   // quaternion incremental rotation
  QuaternionType* m_RotQuater;    // quaternion total rotation
  bool m_IsSpinning;              // if the user did a SPIN
  bool m_IsChanged;               // if the view changed
  bool m_Continous;               // Should we keep applying the transformation
  


}; // Trackball

#endif

