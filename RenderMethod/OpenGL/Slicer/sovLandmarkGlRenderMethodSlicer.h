/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLandmarkGlRenderMethodSlicer.h,v $
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
#ifndef sov_LandmarkGlRenderMethodSlicer_h
#define sov_LandmarkGlRenderMethodSlicer_h

#include "sovGlRenderMethodSlicer.h"
#include "itkLandmarkSpatialObject.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

class SOVOpenGLRenderMethod_EXPORT LandmarkGlRenderMethodSlicer
: public GlRenderMethodSlicer
{

public:


  typedef LandmarkGlRenderMethodSlicer        Self;
  typedef GlRenderMethodSlicer                Superclass;
  typedef SmartPointer< Self >                Pointer;
  typedef SmartPointer< const Self >          ConstPointer;
  typedef Superclass::ObjectType              ObjectType;
  typedef Superclass::ObjectListType          ObjectListType;

  typedef itk::LandmarkSpatialObject<3>       LandmarkType;
  typedef LandmarkType::Pointer               LandmarkPointer;
  typedef LandmarkType::LandmarkPointType     LandmarkPointType;
  typedef LandmarkPointType*                  LandmarkPointPointer;
  typedef LandmarkType::PointListType         LandmarkPointListType;
  typedef LandmarkPointListType *             LandmarkPointListPointer;
  typedef LandmarkPointType::PointType        PointType;
  typedef PointType *                         PointPointer;


  itkNewMacro( LandmarkGlRenderMethodSlicer );

  itkTypeMacro( LandmarkGlRenderMethodSlicer, GlRenderMethodSlicer );

  virtual void draw( void );

protected:

  LandmarkGlRenderMethodSlicer( void );
  ~LandmarkGlRenderMethodSlicer( void );

  unsigned long m_LastDrawTime;

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update(); 
  
};

}

#endif //_LandmarkGlRenderMethodSlicer_h
