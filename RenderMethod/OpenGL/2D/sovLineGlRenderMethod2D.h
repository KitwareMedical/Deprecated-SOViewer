/*=========================================================================

  Program:   itkUNC
  Module:    $RCSfile: sovLineGlRenderMethod2D.h,v $
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
#ifndef sov_LineGlRenderMethod2D_h
#define sov_LineGlRenderMethod2D_h

#include "sovGlRenderMethod2D.h"
#include "itkLineSpatialObject.h"
#include "sovOpenGLWin32Header.h"

namespace sov
{

class SOVOpenGLRenderMethod_EXPORT LineGlRenderMethod2D
: public GlRenderMethod2D
{

public:


  typedef LineGlRenderMethod2D        Self;
  typedef GlRenderMethod2D            Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  typedef Superclass::ObjectType      ObjectType;
  typedef Superclass::ObjectListType  ObjectListType;

  typedef itk::LineSpatialObject<2>           LineType;
  typedef LineType::Pointer                   LinePointer;
  typedef LineType::LinePointType             LinePointType;
  typedef LinePointType*                      LinePointPointer;
  typedef LineType::PointListType             LinePointListType;
  typedef LinePointListType *                 LinePointListPointer;
  typedef LinePointType::PointType            PointType;
  typedef PointType *                         PointPointer;


  itkNewMacro( LineGlRenderMethod2D );

  itkTypeMacro( LineGlRenderMethod2D, GlRenderMethod2D );

  virtual void draw( void );

protected:

  LineGlRenderMethod2D( void );
  ~LineGlRenderMethod2D( void );

  void PrintSelf (std::ostream &os, Indent indent) const;

  virtual void Update(); 
  
};

}

#endif //_LineGlRenderMethod2D_h
