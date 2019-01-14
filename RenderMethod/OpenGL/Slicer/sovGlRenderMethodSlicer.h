/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovGlRenderMethodSlicer.h,v $
  Language:  C++
  Date:      $Date: 2005-01-27 23:31:57 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Julien Jomier

  Copyright (c) 2002 CADDLab @ UNC. All rights reserved.
  See itkUNCCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __GlRenderMethodSlicer_h
#define __GlRenderMethodSlicer_h

#include "sovRenderMethod.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

class SOVOpenGLRenderMethod_EXPORT GlRenderMethodSlicer
: public RenderMethod<3>
{

public:

  typedef GlRenderMethodSlicer          Self;
  typedef RenderMethod<3>               Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef Superclass::ObjectType        ObjectType;
  typedef Superclass::ObjectListType    ObjectListType;
  
  itkNewMacro( GlRenderMethodSlicer );

  itkTypeMacro( GlRenderMethodSlicer, RenderMethodSlicer );

  virtual void draw();
  
  bool IsSupported( const char* object ) const;

  void SetDisplayVolume(unsigned char * volume );

  void SetImageSize(unsigned int x, unsigned int y);

  void SetSliceNumber(unsigned int slice, unsigned int index);

  /** The orientation parameter determines the axis along which you
   * project.  So 2 ==> xy-plane, 0 ==> zy-plane, 1 ==> xz-plane. */
  void SetOrientation(unsigned int orientation);

  void FlipX(bool value) { this->m_FlipX = value; }
  void FlipY(bool value) { this->m_FlipY = value; }

  bool FlipX(){ return this->m_FlipX; }
  bool FlipY(){ return this->m_FlipY; }


protected:

  bool m_FlipX;
  bool m_FlipY;

  GlRenderMethodSlicer();
  
  ~GlRenderMethodSlicer();
 
  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update();

  unsigned char* m_DisplayVolume;
  unsigned char* m_DisplaySlice;
  unsigned int m_SliceSizeX;
  unsigned int m_SliceSizeY;
  unsigned int m_SliceNumber[3];
  unsigned int m_Orientation;
};

}
#endif //__GlRenderMethodSlicer_h
